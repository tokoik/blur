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
  pvLoc = glGetAttribLocation(program, "pv");

  // �ϊ��s��� uniform �ϐ��̏ꏊ
  mcLoc = glGetUniformLocation(program, "mc");
  mwLoc = glGetUniformLocation(program, "mw");
}

void gg::GgPointShader::use(GLuint vert, ...) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::use(0);

  // �ϊ�
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);

  // ���_�����Ƀo�b�t�@�I�u�W�F�N�g���w�肷��
  glBindBuffer(GL_ARRAY_BUFFER, vert);

  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(pvLoc);

  // attribute �ϐ� pv �Ɣz��ϐ� vert �����т���
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void gg::GgPointShader::unuse(void) const
{
  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(pvLoc);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::unuse();
}

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  loadModelViewProjectionMatrix(mp * mw);
  loadModelViewMatrix(mw);
}

void gg::GgPointShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
