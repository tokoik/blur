/*
** �P���ȉA�e�t��
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
  // �v���O������
  GLuint program = get();

  // �T���v���� uniform �ϐ��̏ꏊ
  loc.texture0 = glGetUniformLocation(program, "texture0");
  loc.texture1 = glGetUniformLocation(program, "texture1");
  
  // ������ uniform �ϐ��̏ꏊ
  loc.rn = glGetUniformLocation(program, "rn");

  // �����̔���
  size(0.01f, 0.01f);
}

void gg::GgPass2Shader::size(GLfloat x, GLfloat y)
{
  // �����̔���
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
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgPointShader::use(vert);

  // �T���v��
  glUniform1i(loc.texture0, 0);
  glUniform1i(loc.texture1, 1);
  
  // ����
  glUniform2fv(loc.rn, random.samples, random.rn);
}

void gg::GgPass2Shader::unuse(void) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgPointShader::unuse();
}
