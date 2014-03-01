#pragma once
#include "PreDecl.h"

namespace swift {
namespace predecl {

class GammaDistrDecl: public swift::predecl::PreDecl {
public:
  GammaDistrDecl();
  virtual ~GammaDistrDecl();

  virtual std::shared_ptr<ir::Expr>
  getNew(std::vector<std::shared_ptr<ir::Expr>>& args,
      fabrica::TypeFactory* fact) const;
};

}
}
