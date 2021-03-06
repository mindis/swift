/*
 * Expr.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: leili
 */
#include "Expr.h"

namespace swift {
namespace ir {
Expr::Expr() {
}

Expr::~Expr() {
}

void Expr::addArg(std::shared_ptr<Expr> expr) {
  args.push_back(expr);
}

void Expr::setArgs(std::vector<std::shared_ptr<Expr>> a) {
  args = a;
}

const std::vector<std::shared_ptr<Expr>>& Expr::getArgs() const {
  return args;
}

size_t Expr::argSize() const {
  return args.size();
}

std::shared_ptr<Expr> Expr::get(size_t k) const {
  return args[k];
}

std::string Expr::toString() {
  return "";
}

}
}
