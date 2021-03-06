#include "CardExpr.h"
#include "SetExpr.h"
#include "SetTy.h"

namespace swift {
namespace ir {
CardExpr::CardExpr() :
    referTyp(NULL) {
}

CardExpr::~CardExpr() {
}

const Ty* CardExpr::getType() const {
  return referTyp;
}

void CardExpr::setBody(std::shared_ptr<SetExpr> b) {
  body = b;
  referTyp = ((const SetTy*) b->getTyp())->getRefer();
}

std::shared_ptr<SetExpr> CardExpr::getBody() const {
  return body;
}

void CardExpr::print(FILE* file, int indent) const {
  fprintf(file, "%*sCardExpr:\n", indent, "");
  fprintf(file, "%*sreferTyp: %s\n", indent + 2, "",
      referTyp->toString().c_str());
  fprintf(file, "%*sbody:\n", indent + 2, "");
  getBody()->print(file, indent + 4);
}

std::string CardExpr::toString() {
  return "#" + getBody()->toString();
}

}
}
