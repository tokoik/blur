/*
** �_�����Ƃ��ĕ\������
*/
#include "GgPointShader.h"

gg::GgPointShader::GgPointShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, int vertices,
  int nvarying, const char **varyings)
  : GgShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // �v���O������
  GLuint program = get();

  // �ʒu�� attribute �ϐ��̏ꏊ
  loc.pv = glGetAttribLocation(program, "pv");

  // �ϊ��s��� uniform �ϐ��̏ꏊ
  loc.mc = glGetUniformLocation(program, "mc");
  loc.mw = glGetUniformLocation(program, "mw");
}

void gg::GgPointShader::use(GLuint vert, ...) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::use(0);

  // �ϊ�
  glUniformMatrix4fv(loc.mc, 1, GL_FALSE, m.c);
  glUniformMatrix4fv(loc.mw, 1, GL_FALSE, m.w);

  // ���_�����Ƀo�b�t�@�I�u�W�F�N�g���w�肷��
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(loc.pv);

  // attribute �ϐ� pv �Ɣz��ϐ� vert �����т���
  glVertexAttribPointer(loc.pv, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPointShader::unuse(void) const
{
  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(loc.pv);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::unuse();
}

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  m.loadModelViewProjectionMatrix(mp * mw);
  m.loadModelViewMatrix(mw);
}

void gg::GgPointShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
