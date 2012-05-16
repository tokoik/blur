#ifndef SQUARE_H
#define SQUARE_H

/*
** éläpå`
*/
#include "Shape.h"

class Square : public Shape
{
  GLfloat vert[4][3], norm[4][3];
public:
  Square(void) {}
  Square(const Square &o) : Shape(o) {}
  Square(GLfloat w, GLfloat h) { resize(w, h); }
  virtual ~Square(void) {}
  void resize(GLfloat w, GLfloat h);
  virtual void draw(void) const;
};

#endif