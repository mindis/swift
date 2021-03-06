#include "DirichletSimpleTest2.h"

namespace test_absyn { 

using namespace swift::absyn;

DirichletSimpleTest2::DirichletSimpleTest2() :root(NULL){}

DirichletSimpleTest2::~DirichletSimpleTest2() {
  if (root != NULL) delete root;
}

BlogProgram* DirichletSimpleTest2::getRoot() {
  return root;
}

/*
random RealMatrix w ~ Dirichlet([1.0, 2.0, 3.0]);
random Integer x ~ Discrete(w);
query x;
*/

void DirichletSimpleTest2::build(){
  BlogProgram *blog = new BlogProgram(0, 0);
  root = blog;
  /*
  random RealMatrix w ~ Dirichlet([1.0, 2.0, 3.0]);
  */
  {
    ArrayExpr* arr = new ArrayExpr(0, 0, 1);
    arr->add(new DoubleLiteral(0, 0, 1.0));
    arr->add(new DoubleLiteral(0, 0, 2.0));
    arr->add(new DoubleLiteral(0, 0, 3.0));
    FuncApp *dis = new FuncApp(0, 0, Symbol("Dirichlet"));
    dis->add(arr);
    FuncDecl *fd = new FuncDecl(0, 0, true, Symbol("RealMatrix"), Symbol("w"), dis);
    blog->add(fd);
  }
  /*
  random Integer x ~ Discrete(w);
  */
  {
    FuncApp *dis = new FuncApp(0, 0, Symbol("Discrete"));
    dis->add(new FuncApp(0, 0, Symbol("w")));
    FuncDecl *fd = new FuncDecl(0, 0, true, Symbol("Integer"), Symbol("x"), dis);
    blog->add(fd);
  }
  /*
  query x;
  */
  {
    blog->add(new Query(0, 0, new FuncApp(0, 0, Symbol("x"))));
  }
}

void DirichletSimpleTest2::test(FILE *file)
{
  build();
  if(file != NULL) root->print(file, 0);
}

}
