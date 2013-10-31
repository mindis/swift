/*
 * OpExpr.h
 *
 *  Created on: Oct 29, 2013
 *      Author: yiwu
 *
 *  Note:
 *    When Dealing with Unary Operator e.g. NOT
 *        Left Child should be set NULL
 */

#ifndef OPEXPR_H_
#define OPEXPR_H_

#include "Expr.h"

namespace swift {

class OpExpr: public swift::Expr {
	AbsynConstant op;
public:
  OpExpr(int l, int c, AbsynConstant op, Expr* left, Expr* right);
	virtual ~OpExpr();

	AbsynConstant getOp();
	Expr* getLeft();
	Expr* getRight();

	// For Debugging Use
	void print(FILE* file, int indent);
};

} /* namespace swift */

#endif /* OPEXPR_H_ */
