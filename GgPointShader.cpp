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
  mwLoc = glGetUniformLocation(program, "mw");
  mcLoc = glGetUniformLocation(program, "mc");
  mgLoc = glGetUniformLocation(program, "mg");
}

void gg::GgPointShader::use(const GLfloat (*vert)[3], ...) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::use(0);
  
  // attribute �ϐ� pv ��z��ϐ����瓾�邱�Ƃ�������
  glEnableVertexAttribArray(pvLoc);
  
  // attribute �ϐ� pv �Ɣz��ϐ� vert �����т���
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);

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
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
}

void gg::GgPointShader::loadMatrix(const GgMatrix &mp, const GgMatrix &mw)
{
  loadModelViewPerspectiveMatrix(mp * mw);
  loadModelViewMatrix(mw);
  loadNormalMatrix(mw.normal());
}
