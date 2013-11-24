/*
 * CPPTranslator.h
 *
 *  Created on: Nov 23, 2013
 *      Author: leili
 */

#pragma once
#include <string>
#include "../ir/IRHeader.h"
#include "../code/Code.h"


namespace swift {
namespace codegen {

class CPPTranslator {
public:
  CPPTranslator();
  virtual ~CPPTranslator();
  void translate(swift::ir::BlogModel* model);
  code::Code* getResult() const;

private:
  code::Code* prog; // holder for result target code
  code::ClassDecl* coreCls; // main Class for the sampler;
  code::NamespaceDecl* coreNs; // main namespace
  code::FunctionDecl* coreClsInit; // init function for main class
  void transTypeDomain(std::shared_ptr<ir::TypeDomain> td);
  void transFun(std::shared_ptr<ir::FuncDefn> fd);
  code::Stmt* transClause(std::shared_ptr<ir::Clause> clause);

  static code::QualType mapIRTypeToCodeType(const ir::Ty * ty); // map ir type to code type


  static const code::QualType INT_TYPE;
  static const code::QualType DOUBLE_TYPE;
  static const code::QualType STRING_TYPE;
  static const code::QualType BOOL_TYPE;
  static const std::string DISTINCT_FIELDNAME; // field name inside class for declared type

};

} /* namespace codegen */
} /* namespace swift */

