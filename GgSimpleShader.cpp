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
  
  // 光源のパラメータの uniform 変数の場所
  lposLoc = glGetUniformLocation(program, "lpos");
  lambLoc = glGetUniformLocation(program, "lamb");
  ldiffLoc = glGetUniformLocation(program, "ldiff");
  lspecLoc = glGetUniformLocation(program, "lspec");
  
  // 材質のパラメータの uniform 変数の場所
  kambLoc = glGetUniformLocation(program, "kamb");
  kdiffLoc = glGetUniformLocation(program, "kdiff");
  kspecLoc = glGetUniformLocation(program, "kspec");
  kshiLoc = glGetUniformLocation(program, "kshi");
  
  // 変換行列の uniform 変数の場所
  mwLoc = glGetUniformLocation(program, "mw");
  mgLoc = glGetUniformLocation(program, "mg");
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
  
  // 光源
  glUniform4fv(lposLoc, 1, lpos);
  glUniform4fv(lambLoc, 1, lamb);
  glUniform4fv(ldiffLoc, 1, ldiff);
  glUniform4fv(lspecLoc, 1, lspec);
  
  // 材質
  glUniform4fv(kambLoc, 1, kamb);
  glUniform4fv(kdiffLoc, 1, kdiff);
  glUniform4fv(kspecLoc, 1, kspec);
  glUniform1f(kshiLoc, kshi);
  
  // 変換
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
}

void gg::GgSimpleShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  loadModelViewProjectionMatrix(mp * mw);
  loadModelViewMatrix(mw);
  loadNormalMatrix(mw.normal());
}

void gg::GgSimpleShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
