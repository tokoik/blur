/*
** 点を球として表示する
*/
#include "GgPointShader.h"

gg::GgPointShader::GgPointShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, int vertices,
  int nvarying, const char **varyings)
: GgShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // プログラム名
  GLuint program = get();
  
  // 位置の attribute 変数の場所
  pvLoc = glGetAttribLocation(program, "pv");
  
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
}

void gg::GgPointShader::use(const GLfloat (*vert)[3], ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgShader::use(0);
  
  // attribute 変数 pv を配列変数から得ることを許可する
  glEnableVertexAttribArray(pvLoc);
  
  // attribute 変数 pv と配列変数 vert を結びつける
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);

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

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  loadModelViewPerspectiveMatrix(mp * mw);
  loadModelViewMatrix(mw);
  loadNormalMatrix(mw.normal());
}
