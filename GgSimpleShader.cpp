/*
** �P���ȉA�e�t��
*/
#include <cstdarg>

#include "GgSimpleShader.h"

gg::GgSimpleShader::GgSimpleShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, int vertices,
  int nvarying, const char **varyings)
  : GgPointShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // �v���O������
  GLuint program = get();

  // �@���� attribute �ϐ��̏ꏊ
  nvLoc = glGetAttribLocation(program, "nv");

  // �����̃p�����[�^�� uniform �ϐ��̏ꏊ
  lposLoc = glGetUniformLocation(program, "lpos");
  lambLoc = glGetUniformLocation(program, "lamb");
  ldiffLoc = glGetUniformLocation(program, "ldiff");
  lspecLoc = glGetUniformLocation(program, "lspec");

  // �ގ��̃p�����[�^�� uniform �ϐ��̏ꏊ
  kambLoc = glGetUniformLocation(program, "kamb");
  kdiffLoc = glGetUniformLocation(program, "kdiff");
  kspecLoc = glGetUniformLocation(program, "kspec");
  kshiLoc = glGetUniformLocation(program, "kshi");

  // �ϊ��s��� uniform �ϐ��̏ꏊ
  mgLoc = glGetUniformLocation(program, "mg");
}

void gg::GgSimpleShader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgPointShader::use(vert);

  // �o�b�t�@�I�u�W�F�N�g����f�[�^���擾����
  glBindBuffer(GL_ARRAY_BUFFER, norm);

  // attribute �ϐ� nv ��z��ϐ����瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(nvLoc);

  // attribute �ϐ� nv �Ɣz��ϐ� norm �����т���
  glVertexAttribPointer(nvLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // ����
  glUniform4fv(lposLoc, 1, lpos);
  glUniform4fv(lambLoc, 1, lamb);
  glUniform4fv(ldiffLoc, 1, ldiff);
  glUniform4fv(lspecLoc, 1, lspec);

  // �ގ�
  glUniform4fv(kambLoc, 1, kamb);
  glUniform4fv(kdiffLoc, 1, kdiff);
  glUniform4fv(kspecLoc, 1, kspec);
  glUniform1f(kshiLoc, kshi);

  // �ϊ�
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
}

void gg::GgSimpleShader::unuse(void) const
{
  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(nvLoc);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::unuse();
}

void gg::GgSimpleShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  GgPointShader::loadMatrix(mp, mw);
  loadNormalMatrix(mw.normal());
}

void gg::GgSimpleShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
