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
  loc.nv = glGetAttribLocation(program, "nv");

  // �����̃p�����[�^�� uniform �ϐ��̏ꏊ
  loc.lpos = glGetUniformLocation(program, "lpos");
  loc.lamb = glGetUniformLocation(program, "lamb");
  loc.ldiff = glGetUniformLocation(program, "ldiff");
  loc.lspec = glGetUniformLocation(program, "lspec");

  // �ގ��̃p�����[�^�� uniform �ϐ��̏ꏊ
  loc.kamb = glGetUniformLocation(program, "kamb");
  loc.kdiff = glGetUniformLocation(program, "kdiff");
  loc.kspec = glGetUniformLocation(program, "kspec");
  loc.kshi = glGetUniformLocation(program, "kshi");

  // �ϊ��s��� uniform �ϐ��̏ꏊ
  loc.mg = glGetUniformLocation(program, "mg");
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
  glEnableVertexAttribArray(loc.nv);

  // attribute �ϐ� nv �Ɣz��ϐ� norm �����т���
  glVertexAttribPointer(loc.nv, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // ����
  glUniform4fv(loc.lpos, 1, l.pos);
  glUniform4fv(loc.lamb, 1, l.amb);
  glUniform4fv(loc.ldiff, 1, l.diff);
  glUniform4fv(loc.lspec, 1, l.spec);

  // �ގ�
  glUniform4fv(loc.kamb, 1, k.amb);
  glUniform4fv(loc.kdiff, 1, k.diff);
  glUniform4fv(loc.kspec, 1, k.spec);
  glUniform1f(loc.kshi, k.shi);

  // �ϊ�
  glUniformMatrix4fv(loc.mg, 1, GL_FALSE, m.g);
}

void gg::GgSimpleShader::unuse(void) const
{
  // attribute �ϐ� pv ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(loc.nv);

  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::unuse();
}

void gg::GgSimpleShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  GgPointShader::loadMatrix(mp, mw);
  m.loadNormalMatrix(mw.normal());
}

void gg::GgSimpleShader::loadMatrix(const GLfloat *mp, const GLfloat *mw)
{
  GgMatrix tmp(mp), tmw(mw);
  loadMatrix(tmp, tmw);
}
