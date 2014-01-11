/*
 * CPPTranslator.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: leili
 */

#include "CPPTranslator.h"

namespace swift {
namespace codegen {

const code::Type CPPTranslator::INT_TYPE("int");
const code::Type CPPTranslator::INT_REF_TYPE("int", true);
const code::Type CPPTranslator::DOUBLE_TYPE("double");
const code::Type CPPTranslator::STRING_TYPE("std::string");
const code::Type CPPTranslator::BOOL_TYPE("bool");
const std::string CPPTranslator::DISTINCT_FIELDNAME = "__name_";
const std::string CPPTranslator::DISTRIBUTION_INIT_FUN_NAME = "init";
const std::string CPPTranslator::DISTRIBUTION_GEN_FUN_NAME = "gen";
const std::string CPPTranslator::DISTRIBUTION_LIKELI_FUN_NAME = "likeli";
const std::string CPPTranslator::DISTRIBUTION_LOGLIKELI_FUN_NAME = "loglikeli";
const std::string CPPTranslator::CURRENT_SAMPLE_NUM_VARNAME = "__cur_loop";
const std::string CPPTranslator::RETURN_VAR_NAME = "__ret_value";
const std::string CPPTranslator::MARK_VAR_REF_NAME = "__mark";

// randomness
const std::string CPPTranslator::RANDOM_DEVICE_VAR_NAME = "__random_device";
const code::Type CPPTranslator::RANDOM_ENGINE_TYPE("std::default_random_engine");
const std::string CPPTranslator::RANDOM_ENGINE_VAR_NAME = "__random_engine";
const int CPPTranslator::INIT_SAMPLE_NUM = -1;

namespace {
/**
 * give the name of the type,
 * return the variable name corresponding to the number of objects for this type
 */
std::string getVarOfNumType(std::string name) {
  return "__num_" + name;
}

/**
 * given the name of a variable (can be number var, or random function)
 * return the function name to get the number of objects for this type
 */
std::string getGetterFunName(std::string name) {
  return "__get_" + name;
}

/**
 * given the name of a variable (can be number var, or random function)
 * return the function name to get the likelihood of objects for this type
 */
std::string getLikeliFunName(std::string name) {
  return "__likeli_" + name;
}

/**
 * given the name of a variable (can be number var, or random function)
 * return the variable name to get the number of samples for the current variable
 */
std::string getMarkVarName(std::string name) {
  return "__mark_" + name;
}

/**
 * given the name of a variable (can be number var, or random function)
 * return the variable name to get the value of that var/fun
 */
std::string getValueVarName(std::string name) {
  return "__value_" + name;
}

}

CPPTranslator::CPPTranslator() {
  // TODO Auto-generated constructor stub

}

CPPTranslator::~CPPTranslator() {
  // TODO Auto-generated destructor stub
}

void CPPTranslator::translate(swift::ir::BlogModel* model) {

  code::FieldDecl::createFieldDecl(coreCls, CURRENT_SAMPLE_NUM_VARNAME,
      INT_TYPE);
  for (auto ty : model->getTypes())
    transTypeDomain(ty);
  for (auto fun : model->getRandFuncs())
    transFun(fun);
}




code::Code* CPPTranslator::getResult() const {
  return prog;
}

void CPPTranslator::transTypeDomain(std::shared_ptr<ir::TypeDomain> td) {
  const std::string& name = td->getName();
  code::ClassDecl* cd = code::ClassDecl::createClassDecl(coreNs, name);
  code::FieldDecl::createFieldDecl(cd, DISTINCT_FIELDNAME, STRING_TYPE);
  int len = td->getPreLen();
  std::string numvar = getVarOfNumType(name);
  // create a field in the main class:::    int numvar;
  code::FieldDecl* fd = code::FieldDecl::createFieldDecl(coreCls, numvar,
      INT_TYPE);
  // create a field in the main class:::    int mark_numvar;
  std::string marknumvar = getMarkVarName(numvar);
  fd = code::FieldDecl::createFieldDecl(coreCls, marknumvar, INT_TYPE);
  // add in the init function:::            numvar = len;
  coreClsInit->addStmt(
      new code::BinaryOperator(new code::VarRef(numvar),
          new code::IntegerLiteral(len), code::OpKind::BO_ASSIGN));
  // add in the init function:::            mark_numvar = -1;
  coreClsInit->addStmt(
      new code::BinaryOperator(new code::VarRef(marknumvar),
          new code::IntegerLiteral(INIT_SAMPLE_NUM), code::OpKind::BO_ASSIGN));
  if (len > 0) {
    // create the function for getting number of objects in this instance, i.e. numvar
    code::FunctionDecl* fun = code::FunctionDecl::createFunctionDecl(coreCls,
        getGetterFunName(numvar), INT_TYPE, true);
    fun->addStmt(new code::ReturnStmt(new code::VarRef(numvar)));
    // TODO please add the corresponding distinct name

  }
  // handle number statement
  int numstlen = td->getNumberStmtSize();
  if (numstlen > 0) {
    // TODO create functions for number statement and their likelihood

  }
}

void CPPTranslator::transFun(std::shared_ptr<ir::FuncDefn> fd) {
  const std::string & name = fd->getName();
  std::string getterfunname = getGetterFunName(name);
  code::Type retty = mapIRTypeToCodeType(fd->getRetTyp());
  // translate random function
  // create gettfunname function
  code::FunctionDecl* fun = code::FunctionDecl::createFunctionDecl(coreCls,
      getterfunname, retty);
  fun->setParams(transParamVarDecls(fun, fd->getArgs()));
  if (fd->isRand()) {
    std::string valuevarname = getValueVarName(name);
    std::string markvarname = getMarkVarName(name);
    transFunBody(fun, fd->getBody(), valuevarname, markvarname);

    std::string likelifunname = getLikeliFunName(name);
    code::FunctionDecl* likelifun = code::FunctionDecl::createFunctionDecl(
        coreCls, likelifunname, DOUBLE_TYPE);
    likelifun->setParams(transParamVarDecls(fun, fd->getArgs()));
    transFunBodyLikeli(likelifun, fd->getBody(), valuevarname, markvarname);
  }
}

void CPPTranslator::transFunBody(code::FunctionDecl* fun,
    std::shared_ptr<ir::Clause> clause, std::string valuevarname,
    std::string markvarname) {
  addFunValueRefStmt(fun, valuevarname, RETURN_VAR_NAME);

  addFunValueRefStmt(fun, markvarname, MARK_VAR_REF_NAME);

  // now translating::: if (markvar == current sample num) then return value;
  code::Stmt* st = new code::IfStmt(
      new code::BinaryOperator(new code::VarRef(MARK_VAR_REF_NAME),
          new code::VarRef(CURRENT_SAMPLE_NUM_VARNAME), code::OpKind::BO_EQU),
      new code::ReturnStmt(new code::VarRef(RETURN_VAR_NAME)), NULL);
  fun->addStmt(st);
  // now should sample
  // mark the variable first
  st = new code::BinaryOperator(new code::VarRef(MARK_VAR_REF_NAME),
      new code::VarRef(CURRENT_SAMPLE_NUM_VARNAME), code::OpKind::BO_ASSIGN);
  fun->addStmt(st);
  //now translate actual sampling part
  transClause(clause, RETURN_VAR_NAME);
  // now return the value
  fun->addStmt(new code::ReturnStmt(new code::VarRef(RETURN_VAR_NAME)));
}

code::Stmt* CPPTranslator::transClause(std::shared_ptr<ir::Clause> clause,
    std::string retvar) {
  std::shared_ptr<ir::Branch> br = std::dynamic_pointer_cast<ir::Branch>(
      clause);
  if (br) {
    return transBranch(br, retvar);
  }
  std::shared_ptr<ir::IfThen> ith = std::dynamic_pointer_cast<ir::IfThen>(
      clause);
  if (ith) {
    return transIfThen(ith, retvar);
  }
  std::shared_ptr<ir::Expr> expr = std::dynamic_pointer_cast<ir::Expr>(clause);
  if (expr) {
    code::CompoundStmt* cst = new code::CompoundStmt();
    cst->addStmt(
        new code::BinaryOperator(new code::VarRef(retvar), transExpr(expr),
            code::OpKind::BO_ASSIGN));
    // TODO no 100% correct here
    return cst;
  }
  return NULL;
}

code::ParamVarDecl* CPPTranslator::transParamVarDecl(code::DeclContext* context,
    const std::shared_ptr<ir::VarDecl> var) {
  return new code::ParamVarDecl(context, var->getVar(),
      mapIRTypeToCodeType(var->getTyp()));
}

std::vector<code::ParamVarDecl*> CPPTranslator::transParamVarDecls(
    code::DeclContext* context,
    const std::vector<std::shared_ptr<ir::VarDecl> > & vars) {
  std::vector<code::ParamVarDecl*> vds;
  for (auto v : vars) {
    vds.push_back(transParamVarDecl(context, v));
  }
  return vds;
}

code::Stmt* CPPTranslator::transBranch(std::shared_ptr<ir::Branch> br,
    std::string retvar) {
  code::SwitchStmt* sst = new code::SwitchStmt(transExpr(br->getVar()));
  code::CaseStmt* cst;
  for (size_t i = 0; i < br->size(); i++) {
    cst = new code::CaseStmt(transExpr(br->getCond(i)),
        transClause(br->getBranch(i), retvar));
    sst->addStmt(cst);
    sst->addStmt(new code::BreakStmt());
  }
  return sst;
}

code::Stmt* CPPTranslator::transIfThen(std::shared_ptr<ir::IfThen> ith,
    std::string retvar) {
  // TODO translate ifthenelse
  return NULL;
}

code::Expr* CPPTranslator::transExpr(std::shared_ptr<ir::Expr> expr) {
  std::vector<code::Expr*> args;
  for (size_t k = 0; k < expr->argSize(); k++) {
    args.push_back(transExpr(expr->get(k)));
  }
  std::shared_ptr<ir::Distribution> dist = std::dynamic_pointer_cast<
      ir::Distribution>(expr);
  if (dist) {
    return transDistribution(dist, args);
  }
  // TODO translate other expression
  return NULL;
}

code::Expr* CPPTranslator::transDistribution(
    std::shared_ptr<ir::Distribution> dist, std::vector<code::Expr*> args) {
  std::string name = dist->getDistrName();
  std::string distname = name + std::to_string((size_t) dist.get());
  // define a field in the main class corresponding to the distribution
  code::FieldDecl::createFieldDecl(coreCls, distname, code::Type(name));
  //put initialization in coreClasInit
  coreClsInit->addStmt(
      new code::CallExpr(
          new code::BinaryOperator(new code::VarRef(distname),
              new code::VarRef(DISTRIBUTION_INIT_FUN_NAME),
              code::OpKind::BO_FIELD), args));
  // now actual sampling a value from the distribution
  std::vector<code::Expr *> rd;
  rd.push_back(new code::VarRef(RANDOM_ENGINE_VAR_NAME));
  return new code::CallExpr(
      new code::BinaryOperator(new code::VarRef(distname),
          new code::VarRef(DISTRIBUTION_GEN_FUN_NAME), code::OpKind::BO_FIELD),
      rd);
}

void CPPTranslator::createInit() {
  code::FieldDecl::createFieldDecl(coreCls, RANDOM_DEVICE_VAR_NAME, RANDOM_ENGINE_TYPE);
}



void CPPTranslator::transFunBodyLikeli(code::FunctionDecl* fun,
    std::shared_ptr<ir::Clause> clause, std::string valuevarname,
    std::string markvarname) {

  addFunValueRefStmt(fun, valuevarname, RETURN_VAR_NAME);

  // TODO add likelihood calculation
  // now translating::: if (markvar == current sample num) then return value;
  code::Stmt* st = new code::IfStmt(
      new code::BinaryOperator(new code::VarRef(MARK_VAR_REF_NAME),
          new code::VarRef(CURRENT_SAMPLE_NUM_VARNAME), code::OpKind::BO_EQU),
      new code::ReturnStmt(new code::VarRef(RETURN_VAR_NAME)), NULL);
  fun->addStmt(st);
  // now should sample
  // mark the variable first
  st = new code::BinaryOperator(new code::VarRef(MARK_VAR_REF_NAME),
      new code::VarRef(CURRENT_SAMPLE_NUM_VARNAME), code::OpKind::BO_ASSIGN);
  fun->addStmt(st);
  //now translate actual sampling part
  transClause(clause, RETURN_VAR_NAME);
  // now return the value
  fun->addStmt(new code::ReturnStmt(new code::VarRef(RETURN_VAR_NAME)));
}

void CPPTranslator::addFunValueRefStmt(code::FunctionDecl* fun,
    std::string valuevarname, std::string valuerefname) {
  // for value_var
  code::Expr* exp = new code::VarRef(valuevarname);
  for (auto prm : fun->getParams()) {
    exp = new code::ArraySubscriptExpr(exp, new code::VarRef(prm->getId()));
  }

  code::VarDecl* retvar = new code::VarDecl(fun, valuerefname, INT_REF_TYPE,
      exp); // todo support for other return type
  code::DeclStmt* dst = new code::DeclStmt(retvar);
  fun->addStmt(dst);
}

code::Type CPPTranslator::mapIRTypeToCodeType(const ir::Ty* ty) {
  switch (ty->getTyp()) {
  case ir::IRConstant::BOOL:
    return BOOL_TYPE;
  case ir::IRConstant::INT:
    return INT_TYPE;
  case ir::IRConstant::DOUBLE:
    return DOUBLE_TYPE;
  case ir::IRConstant::STRING:
    return STRING_TYPE;
  default:
    return INT_TYPE; // all declared type return int type
  }
}

} /* namespace codegen */
} /* namespace swift */
