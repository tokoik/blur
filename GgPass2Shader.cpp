/*
** モーションブラー (Pass 2)
*/
#include <cstdarg>

#include "GgPass2Shader.h"

gg::GgPass2Shader::GgPass2Shader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices)
  : GgShader(vert, frag, geom, input, output, vertices)
{
  // プログラム名
  GLuint program = get();

  // 以前の頂点のスクリーン上の位置 p1 の attribute 変数の場所
  loc.p0 = glGetAttribLocation(program, "p0");

  // 現在の頂点のスクリーン上の位置 p1 の attribute 変数の場所
  loc.p1 = glGetAttribLocation(program, "p1");
}

void gg::GgPass2Shader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint p1 = va_arg(list, GLuint);
  va_end(list);

  // 基底クラスのシェーダの設定を呼び出す
  GgShader::use(0);

  // 頂点属性にバッファオブジェクト vert を使用する
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute 変数 p0 をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(loc.p0);

  // attribute 変数 p0 とバッファオブジェクトを結びつける
  glVertexAttribPointer(loc.p0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  // 頂点属性にバッファオブジェクト p1 を使用する
  glBindBuffer(GL_ARRAY_BUFFER, p1);

  // attribute 変数 p1 をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(loc.p1);

  // attribute 変数 p1 とバッファオブジェクトを結びつける
  glVertexAttribPointer(loc.p1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPass2Shader::unuse(void) const
{
  // attribute 変数 p0 をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.p0);

  // attribute 変数 p1 をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.p1);

  // 基底クラスのシェーダの設定を呼び出す
  GgShader::unuse();
}
