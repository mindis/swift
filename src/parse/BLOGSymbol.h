#pragma once
#include <iostream>
using namespace std;
template<class Object>
class BLOGSymbol {
  int line;
  int column;
  Object val;
public:
  BLOGSymbol(int l, int c, Object v) {
    line = l;
    column = c;
    val = v;
  }
  int getLine(void) {
    return line;
  }
  int getColumn(void) {
    return column;
  }
  Object getValue(void) {
    return val;
  }
};
