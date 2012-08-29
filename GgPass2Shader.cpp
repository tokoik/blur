/*
** 単純な陰影付け
*/
#include <cstdlib>
#include <cstdarg>
#include <cmath>

#include "GgPass2Shader.h"

gg::GgPass2Shader::GgPass2Shader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices,
  GLint nvarying, const char **varyings)
  : GgPointShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // プログラム名
  GLuint program = get();

  // サンプラの uniform 変数の場所
  loc.texture0 = glGetUniformLocation(program, "texture0");
  loc.texture1 = glGetUniformLocation(program, "texture1");
  
  // 乱数の uniform 変数の場所
  loc.rn = glGetUniformLocation(program, "rn");

  // 乱数の発生
  size(0.01f, 0.01f);
}

void gg::GgPass2Shader::size(GLfloat x, GLfloat y)
{
  // 乱数の発生
  for (unsigned int i = 0; i < random.samples * 2; i += 2)
  {
    GLfloat r = sqrt(2.0f * (float)rand() / ((float)RAND_MAX + 1.0f));
    GLfloat t = 6.283185f * (float)rand() / ((float)RAND_MAX + 1.0f);
    random.rn[i + 0] = x * r * cos(t);
    random.rn[i + 1] = y * r * sin(t);
  }
}

void gg::GgPass2Shader::use(GLuint vert, ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgPointShader::use(vert);

  // サンプラ
  glUniform1i(loc.texture0, 0);
  glUniform1i(loc.texture1, 1);
  
  // 乱数
  glUniform2fv(loc.rn, random.samples, random.rn);
}

void gg::GgPass2Shader::unuse(void) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgPointShader::unuse();
}
