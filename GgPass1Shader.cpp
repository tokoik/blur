/*
** モーションブラー
*/
#include <iostream>
#include <fstream>
#include <cstdarg>

#include "GgPass1Shader.h"

const char *gg::GgPass1Shader::varyings[] = { "p1" };

gg::GgPass1Shader::GgPass1Shader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices)
  : GgSimpleShader(vert, frag, geom, input, output, vertices, 1, varyings)
{
  // プログラム名
  GLuint program = get();
  
  // フィードバックバッファからの読み出し用の attribute 変数の場所
  loc.p0 = glGetAttribLocation(program, "p0");
  
  // 最初に使用するバッファ
  b.buffer = 0;
}

void gg::GgPass1Shader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::use(vert, norm);
  
  // フィードバックのソースとなるバッファオブジェクトを選択する
  glBindBuffer(GL_ARRAY_BUFFER, b.fb[b.buffer].buf());
  
  // attribute 変数 p0 をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(loc.p0);
  
  // attribute 変数 nv とバッファオブジェクトを結びつける
  glVertexAttribPointer(loc.p0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  
  // フィードバックのターゲットとなるバッファオブジェクトを選択する
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, b.fb[1 - b.buffer].buf());
  
  // Transform Feedback 開始
  glBeginTransformFeedback(GL_TRIANGLES);
}

void gg::GgPass1Shader::unuse(void) const
{
  // Transform Feedback 終了
  glEndTransformFeedback();
  
  // attribute 変数 p0 をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.p0);
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::unuse();
}

void gg::GgPass1Shader::copyBuffer(GLuint num, GLuint buf)
{
  // フィードバックバッファ用の頂点バッファオブジェクトのメモリの確保
  b.fb[b.buffer].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[1 - b.buffer].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[b.buffer].copy(buf);
}
