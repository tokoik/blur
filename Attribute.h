#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

/*
** ����
*/
#include "gg.h"

class Attribute {
  int count;        // �Q�ƃJ�E���g
public:
  Attribute(void) { count = 0; }
  virtual ~Attribute(void) {}
  int inc(void) { return ++count; }
  int dec(void) { return --count; }
};

#endif
