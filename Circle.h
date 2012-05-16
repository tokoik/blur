#ifndef CIRCLE_H
#define CIRCLE_H

/*
** ‘È‰~Œ`
*/
#include "Object.h"

#define SLICES 16

class Circle : public Object
{
public:
  Circle(void) {}
  Circle(const Circle &o) : Object(o) {}
  Circle(GLfloat w, GLfloat h, int slices = SLICES) { resize(w, h, slices); }
  virtual ~Circle(void) {}
  void resize(GLfloat w, GLfloat h, int slices = SLICES);
};

#endif
