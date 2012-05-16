#ifndef SHADER_H
#define SHADER_H

/*
** シェーダプログラム
*/
#include "Attribute.h"

class Shader : public Attribute {
protected:
  GLuint program;   // プログラム名
public:
  Shader(void) { program = 0; }
  Shader(const char *vert, const char *frag, const char *geom = 0) { load(vert, frag, geom); }
  virtual ~Shader(void) { if (program) glDeleteProgram(program); }
  bool load(const char *vert, const char *frag, const char *geom = 0);
  virtual void use(const GLfloat (*vert)[3], ...) const { glUseProgram(program); }
  void unuse(void) const { glUseProgram(0); }
};

#endif
