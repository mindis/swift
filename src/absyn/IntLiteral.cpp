/*
* IntLiteral.cpp
*
*  Created on: Oct 29, 2013
*      Author: yiwu
*/

#include "IntLiteral.h"

namespace swift{

IntLiteral::IntLiteral(int l, int c, int value)
	:Literal(l, c, AbsynLiteralConstant::INT), value(value) {
}

IntLiteral::~IntLiteral() {
}

int IntLiteral::getValue() {
	return value;
}

}