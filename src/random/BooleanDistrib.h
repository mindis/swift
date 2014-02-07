/*
 * BooleanDistrib.h
 *
 *  Created on: Feb 6, 2014
 *      Author: yiwu
 */

#pragma once
#include <random>
#include "SwiftDistribution.h"

namespace swift {
namespace random {

class BooleanDistrib: public swift::random::SwiftDistribution<bool> {
public:
  BooleanDistrib();
  virtual ~BooleanDistrib();
  void init(double p);
  bool gen();
  double likeli(bool x);
  double loglikeli(bool x);
private:
  std::bernoulli_distribution dist;
  double p;
  double logp;
  double log1_p;
};

} /* namespace random */
} /* namespace swift */