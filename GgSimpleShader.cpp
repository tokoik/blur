/*
** 単純な陰影付け
*/
#include <cstdarg>

#include "GgSimpleShader.h"

gg::GgSimpleShader::GgSimpleShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, int vertices,
  int nvarying, const char **varyings)
: GgPointShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // プログラム名
  GLuint program = get();

  // 法線の attribute 変数の場所
  nvLoc = glGetAttribLocation(program, "nv");
}

void gg::GgSimpleShader::use(const GLfloat (*vert)[3], ...) const
{
  va_list list;
  va_start(list, vert);
  const GLfloat *norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  GgPointShader::use(vert);
  
  // attribute 変数 nv を配列変数から得ることを許可する
  glEnableVertexAttribArray(nvLoc);
  
  // attribute 変数 nv と配列変数 norm を結びつける
  glVertexAttribPointer(nvLoc, 3, GL_FLOAT, GL_FALSE, 0, norm);
}
