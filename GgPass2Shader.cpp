/*
** �P���ȉA�e�t��
*/
#include <cstdarg>

#include "GgPass2Shader.h"

gg::GgPass2Shader::GgPass2Shader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices,
  GLint nvarying, const char **varyings)
  : GgPointShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // �v���O������
  GLuint program = get();

  // �T���v���� uniform �ϐ��̏ꏊ
  loc.texture0 = glGetUniformLocation(program, "texture0");
  loc.texture1 = glGetUniformLocation(program, "texture1");
}

void gg::GgPass2Shader::use(GLuint vert, ...) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgPointShader::use(vert);

  // �T���v��
  glUniform1i(loc.texture0, 0);
  glUniform1i(loc.texture1, 1);
}

void gg::GgPass2Shader::unuse(void) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgPointShader::unuse();
}
