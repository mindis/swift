/*
 * Configuration.cpp
 *
 *  Created on: Jan 28, 2014
 *      Author: leili
 */

#include "Configuration.h"

namespace swift {

Configuration* Configuration::config = NULL;

Configuration::Configuration() {
  setValue("COMPUTE_LIKELIHOOD_IN_LOG", "true");
}

Configuration* Configuration::getConfiguration() {
  if (!config)
    config = new Configuration();
  return config;
}

bool Configuration::getBoolValue(std::string name) {
  return getValue(name) == "true";
}

int Configuration::getIntValue(std::string name) {
  return std::stoi(getValue(name));
}

Configuration::~Configuration() {
}

template<typename _T>
void Configuration::setValue(std::string name, _T value) {
  property[name] = std::to_string(value);
}

std::string Configuration::getValue(std::string name) {
  auto x = property.find(name);
  if (x == property.end())
    return "";
  return x->second;
}

}
