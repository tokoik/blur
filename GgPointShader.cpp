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

  // 変換行列の uniform 変数の場所
  mcLoc = glGetUniformLocation(program, "mc");
  mwLoc = glGetUniformLocation(program, "mw");
}

void gg::GgPointShader::use(GLuint vert, ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgShader::use(0);

  // 変換
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);

  // 頂点属性にバッファオブジェクトを指定する
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute 変数 pv をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(pvLoc);

  // attribute 変数 pv と配列変数 vert を結びつける
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPointShader::unuse(void) const
{
  // attribute 変数 pv をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(pvLoc);

  // 基底クラスのシェーダの設定を呼び出す
  GgShader::unuse();
}

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  loadModelViewProjectionMatrix(mp * mw);
  loadModelViewMatrix(mw);
}

void gg::GgPointShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
