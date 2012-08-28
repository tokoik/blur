/*
** 単純な陰影付け
*/
#include <cstdarg>

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
}

void gg::GgPass2Shader::use(GLuint vert, ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgPointShader::use(vert);

  // サンプラ
  glUniform1i(loc.texture0, 0);
  glUniform1i(loc.texture1, 1);
}

void gg::GgPass2Shader::unuse(void) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgPointShader::unuse();
}
