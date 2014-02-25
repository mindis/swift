#pragma once

#include <string>
#include <vector>

#include "Expr.h"
#include "IRForwardDecl.h"

// Forward Declaration
namespace swift {
namespace predecl {
class PreDecl;
}
}

namespace swift {
namespace ir {

class Distribution: public swift::ir::Expr {
public:
  Distribution(const std::string& name = std::string(""),
      const predecl::PreDecl* refer = NULL);
  virtual ~Distribution();

  const predecl::PreDecl* getRefer() const;

  const std::string& getDistrName() const;
  void print(FILE* file, int indent) const;

private:
  const predecl::PreDecl* refer;
  std::string name;
};

}
}
