#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

/*
** 属性
*/
#include "gg.h"

class Attribute {
  int count;        // 参照カウント
public:
  Attribute(void) { count = 0; }
  virtual ~Attribute(void) {}
  int inc(void) { return ++count; }
  int dec(void) { return --count; }
};

#endif
