#pragma once

#include <vector>

#include "Clause.h"
#include "IRForwardDecl.h"

namespace swift {
namespace ir {

class Branch: public swift::ir::Clause {
public:
  Branch();
  virtual ~Branch();

  void setVar(std::shared_ptr<Expr> v);
  std::shared_ptr<Expr> getVar() const;
  void addBranch(std::shared_ptr<Expr> c, std::shared_ptr<Clause> b);
  size_t size() const;
  std::shared_ptr<Expr> getCond(size_t k) const;
  const std::vector<std::shared_ptr<Expr>>& getConds() const;
  std::shared_ptr<Clause> getBranch(size_t k) const;
  const std::vector<std::shared_ptr<Clause>>& getBranches() const;
  virtual void print(FILE* file, int indent) const;

  // When dim = 1, it is the normal case expression
  //      dim > 1, it is the Multi-case expression
  void setArgDim(int _dim);
  int getArgDim();

private:
  std::shared_ptr<Expr> var;
  std::vector<std::shared_ptr<Expr>> cond;
  std::vector<std::shared_ptr<Clause>> branch;
  int dim;
};

}
}
