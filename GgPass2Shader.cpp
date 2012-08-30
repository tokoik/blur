/*
** ���[�V�����u���[ (Pass 2)
*/
#include <cstdarg>

#include "GgPass2Shader.h"

gg::GgPass2Shader::GgPass2Shader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices)
  : GgShader(vert, frag, geom, input, output, vertices)
{
  // �v���O������
  GLuint program = get();

  // �ȑO�̒��_�̃X�N���[����̈ʒu p1 �� attribute �ϐ��̏ꏊ
  loc.p0 = glGetAttribLocation(program, "p0");

  // ���݂̒��_�̃X�N���[����̈ʒu p1 �� attribute �ϐ��̏ꏊ
  loc.p1 = glGetAttribLocation(program, "p1");
}

void gg::GgPass2Shader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint p1 = va_arg(list, GLuint);
  va_end(list);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::use(0);

  // ���_�����Ƀo�b�t�@�I�u�W�F�N�g vert ���g�p����
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute �ϐ� p0 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(loc.p0);

  // attribute �ϐ� p0 �ƃo�b�t�@�I�u�W�F�N�g�����т���
  glVertexAttribPointer(loc.p0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  // ���_�����Ƀo�b�t�@�I�u�W�F�N�g p1 ���g�p����
  glBindBuffer(GL_ARRAY_BUFFER, p1);

  // attribute �ϐ� p1 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(loc.p1);

  // attribute �ϐ� p1 �ƃo�b�t�@�I�u�W�F�N�g�����т���
  glVertexAttribPointer(loc.p1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPass2Shader::unuse(void) const
{
  // attribute �ϐ� p0 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(loc.p0);

  // attribute �ϐ� p1 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(loc.p1);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::unuse();
}
