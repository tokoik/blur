/*
** 点を球として表示する
*/
#include "GgPointShader.h"

gg::GgPointShader::GgPointShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices,
  GLint nvarying, const char **varyings)
  : GgShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // プログラム名
  GLuint program = get();

  // 位置の attribute 変数の場所
  loc.pv = glGetAttribLocation(program, "pv");

  // 変換行列の uniform 変数の場所
  loc.mc = glGetUniformLocation(program, "mc");
  loc.mw = glGetUniformLocation(program, "mw");
}

void gg::GgPointShader::use(GLuint vert, ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgShader::use(0);

  // 変換
  glUniformMatrix4fv(loc.mc, 1, GL_FALSE, m.c);
  glUniformMatrix4fv(loc.mw, 1, GL_FALSE, m.w);

  // バッファオブジェクト vert を頂点属性に使用する
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute 変数 pv をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(loc.pv);

  // attribute 変数 pv とバッファオブジェクトを結びつける
  glVertexAttribPointer(loc.pv, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPointShader::unuse(void) const
{
  // attribute 変数 pv をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.pv);

  // 基底クラスのシェーダの設定を呼び出す
  GgShader::unuse();
}

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  m.loadModelViewProjectionMatrix(mp * mw);
  m.loadModelViewMatrix(mw);
}

void gg::GgPointShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
