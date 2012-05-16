#ifndef SHAPE_H
#define SHAPE_H

/*
** 形状データ
*/
#include "Shader.h"

class Shape
{
  Shader *shader;   // シェーダー
  GLenum cull;      // カリング面
  void init(void) { shader = 0; cull = GL_BACK; }
  void copy(const Shape &o) { shader = o.shader; if (shader) shader->inc(); }
  void free(void) { if (shader && shader->dec() <= 0) delete shader; }
public:
  Shape(void) { init(); }
  Shape(const Shape &o) { copy(o); }
  virtual ~Shape(void) { free(); }
  virtual Shape &operator=(const Shape &o) { if (this != &o) { copy(o); Shape::copy(o); } return *this; }
  void setShader(Shader *s) { if (shader && shader->dec() <= 0) delete shader; shader = s; if (shader) shader->inc(); }
  const Shader *getShader(void) const { return shader; }
  void setCullFace(bool f) { cull = f ? GL_BACK : GL_FRONT; }
  GLenum getCullFace(void) const { return cull; };
  virtual void draw(void) const {};
};

#endif
