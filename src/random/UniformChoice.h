/*
 * UniformChoice.h
 *
 *  Created on: Feb 8, 2014
 *      Author: leili
 */

#pragma once
#include <vector>
#include <random>
#include <map>
#include <unordered_set>
#include <vector>
#include "SwiftDistribution.h"

namespace swift {
namespace random {

class UniformChoice: public swift::random::SwiftDistribution<int> {
public:
  UniformChoice();
  ~UniformChoice();
  void init(std::vector<int> values);
  int gen();
  template<typename _RD>
  int gen(_RD& rd);
  double likeli(const int& x);
  double loglikeli(const int& x);
private:
  std::uniform_int_distribution<int> dist;
  std::vector<int> values; // values that this distribution will generation
  double weight, log_weight;
  std::unordered_set<int> ind; // value appear at which index
};

} /* namespace random */
} /* namespace swift */