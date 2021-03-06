#pragma once
#include "Expr.h"
#include "IRForwardDecl.h"

namespace swift {
namespace ir {
class MatrixExpr: public swift::ir::Expr {
public:
  MatrixExpr();
  virtual ~MatrixExpr();

  virtual std::string toString();
  // For Debugging Use
  void print(FILE* file, int indent) const;

  void setRowVecFlag(bool flag);
  bool isRowVec();
  void setColVecFlag(bool flag);
  bool isColVec();

private:
  bool flag_colvec;
  bool flag_rowvec;
};

}
}
