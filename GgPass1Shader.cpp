/*
** モーションブラー (Pass 1)
*/
#include <cstdarg>

#include "GgPass1Shader.h"

const char *gg::GgPass1Shader::varyings[] = { "p1" };

void gg::GgPass1Shader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::use(vert, norm);
  
  // フィードバックのターゲットとなるバッファオブジェクトを選択する
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, b.fb[b.select].buf());
  
  // Transform Feedback 開始
  glBeginTransformFeedback(GL_TRIANGLES);
}

void gg::GgPass1Shader::unuse(void) const
{
  // Transform Feedback 終了
  glEndTransformFeedback();
  
  // 基底クラスのシェーダの設定を呼び出す
  GgSimpleShader::unuse();
}

void gg::GgPass1Shader::copyBuffer(GLuint num, GLuint buf)
{
  // フィードバックバッファ用の頂点バッファオブジェクトのメモリの確保
  b.fb[b.select].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[1 - b.select].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[b.select].copy(buf);
}
