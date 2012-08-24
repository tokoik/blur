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
  loc.nv = glGetAttribLocation(program, "nv");

  // 光源のパラメータの uniform 変数の場所
  loc.lpos = glGetUniformLocation(program, "lpos");
  loc.lamb = glGetUniformLocation(program, "lamb");
  loc.ldiff = glGetUniformLocation(program, "ldiff");
  loc.lspec = glGetUniformLocation(program, "lspec");

  // 材質のパラメータの uniform 変数の場所
  loc.kamb = glGetUniformLocation(program, "kamb");
  loc.kdiff = glGetUniformLocation(program, "kdiff");
  loc.kspec = glGetUniformLocation(program, "kspec");
  loc.kshi = glGetUniformLocation(program, "kshi");

  // 変換行列の uniform 変数の場所
  loc.mg = glGetUniformLocation(program, "mg");
}

void gg::GgSimpleShader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);

  // 基底クラスのシェーダの設定を呼び出す
  GgPointShader::use(vert);

  // バッファオブジェクトからデータを取得する
  glBindBuffer(GL_ARRAY_BUFFER, norm);

  // attribute 変数 nv を配列変数から得ることを有効にする
  glEnableVertexAttribArray(loc.nv);

  // attribute 変数 nv と配列変数 norm を結びつける
  glVertexAttribPointer(loc.nv, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // 光源
  glUniform4fv(loc.lpos, 1, l.pos);
  glUniform4fv(loc.lamb, 1, l.amb);
  glUniform4fv(loc.ldiff, 1, l.diff);
  glUniform4fv(loc.lspec, 1, l.spec);

  // 材質
  glUniform4fv(loc.kamb, 1, k.amb);
  glUniform4fv(loc.kdiff, 1, k.diff);
  glUniform4fv(loc.kspec, 1, k.spec);
  glUniform1f(loc.kshi, k.shi);

  // 変換
  glUniformMatrix4fv(loc.mg, 1, GL_FALSE, m.g);
}

void gg::GgSimpleShader::unuse(void) const
{
  // attribute 変数 pv をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.nv);

  // 基底クラスのシェーダの設定を呼び出す
  GgShader::unuse();
}

void gg::GgSimpleShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  GgPointShader::loadMatrix(mp, mw);
  m.loadNormalMatrix(mw.normal());
}

void gg::GgSimpleShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
