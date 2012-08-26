/*
** モーションブラー
*/
#include <cstdarg>

#include "GgBlurShader.h"

const char *gg::GgBlurShader::varyings[] = { "p1" };

gg::GgBlurShader::GgBlurShader(const char *vert, const char *frag,
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

void gg::GgBlurShader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::use(vert, norm);
  
  // フィードバックのソースとなるバッファオブジェクトを選択する
  glBindBuffer(GL_ARRAY_BUFFER, b.fb[b.buffer].buf());
  ggError("fb0");
  
  // attribute 変数 p0 をバッファオブジェクトから得ることを有効にする
  glEnableVertexAttribArray(loc.p0);
  ggError("p0");
  
  // attribute 変数 nv とバッファオブジェクトを結びつける
  glVertexAttribPointer(loc.p0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  ggError("fb0a");
  
  // フィードバックのターゲットとなるバッファオブジェクトを選択する
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, b.fb[1 - b.buffer].buf());
  ggError("fb1");
  
  // Transform Feedback 開始
  glBeginTransformFeedback(GL_TRIANGLES);
  ggError("begin");
}

void gg::GgBlurShader::unuse(void) const
{
  // Transform Feedback 終了
  glEndTransformFeedback();
  
  // attribute 変数 p0 をバッファオブジェクトから得ることを無効にする
  glDisableVertexAttribArray(loc.p0);
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::unuse();
}

void gg::GgBlurShader::copyBuffer(GLuint num, GLuint buf)
{
  // フィードバックバッファ用の頂点バッファオブジェクトのメモリの確保
  b.fb[b.buffer].load(GL_ARRAY_BUFFER, num, 0);
  b.fb[1 - b.buffer].load(GL_ARRAY_BUFFER, num, 0);
  b.fb[b.buffer].copy(buf);
}
