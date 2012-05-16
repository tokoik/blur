/*
** 単純な陰影付け
*/
#include <cstdarg>

#include "SimpleShader.h"

SimpleShader::SimpleShader(const char *vert, const char *frag, const char *geom) : Shader(vert, frag, geom)
{
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
  mcLoc = glGetUniformLocation(program, "mc");
  mgLoc = glGetUniformLocation(program, "mg");

  // attribute 変数の場所
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
}

void SimpleShader::use(const GLfloat (*vert)[3], ...) const
{
  const GLfloat *norm;
  va_list list;
  va_start(list, vert);
  norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  Shader::use(vert, norm);
  
  // attribute 変数 pv, nv を配列変数から得ることを許可する
  glEnableVertexAttribArray(pvLoc);
  glEnableVertexAttribArray(nvLoc);
  
  // attribute 変数 pv, nv と配列変数 vert, norm を結びつける
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);
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
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
}

void SimpleShader::loadMatrix(const Matrix &mp, const Matrix &mw)
{
  loadModelViewPerspectiveMatrix(mp * mw);
  loadModelViewMatrix(mw);
  loadNormalMatrix(mw.normal());
}
