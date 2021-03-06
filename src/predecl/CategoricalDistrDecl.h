#pragma once
#include "PreDecl.h"

namespace swift {
namespace predecl {

class CategoricalDistrDecl: public swift::predecl::PreDecl {
public:
  CategoricalDistrDecl();
  virtual ~CategoricalDistrDecl();

  virtual std::shared_ptr<ir::Expr>
  getNew(std::vector<std::shared_ptr<ir::Expr>>& args,
      fabrica::TypeFactory* fact) const;
};

}
}
