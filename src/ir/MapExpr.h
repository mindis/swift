#pragma once
#include "Expr.h"
#include "IRForwardDecl.h"

namespace swift {
namespace ir {
class MapExpr: public swift::ir::Expr {
public:
  MapExpr();
  virtual ~MapExpr();

  size_t mapSize() const;
  void addMap(std::shared_ptr<Expr> a, std::shared_ptr<Expr> b);
  std::shared_ptr<Expr> getFrom(size_t k) const;
  std::shared_ptr<Expr> getTo(size_t k) const;
  const Ty* getFromTyp() const;
  const Ty* getToTyp() const;
  void setFromTyp(const Ty* t);
  void setToTyp(const Ty* t);

  virtual std::string toString();
  void print(FILE* file, int indent) const;

private:
  const Ty* fromTyp;
  const Ty* toTyp;
};

}
}
