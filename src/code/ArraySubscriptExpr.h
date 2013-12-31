/*
 * ArraySubscriptExpr.h
 *
 *  Created on: Nov 24, 2013
 *      Author: leili
 */

#pragma once

#include "Expr.h"

namespace swift {
namespace code {

class ArraySubscriptExpr: public swift::code::Expr {
public:
  ArraySubscriptExpr(Expr* lhs, Expr* rhs);
  ~ArraySubscriptExpr();
private:
  Expr* lhs;
  Expr* rhs;
};

} /* namespace code */
} /* namespace swift */
