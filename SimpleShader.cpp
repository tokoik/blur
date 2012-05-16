/*
** �P���ȉA�e�t��
*/
#include <cstdarg>

#include "SimpleShader.h"

SimpleShader::SimpleShader(const char *vert, const char *frag, const char *geom) : Shader(vert, frag, geom)
{
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

  // attribute �ϐ��̏ꏊ
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
}

void SimpleShader::use(const GLfloat (*vert)[3], ...) const
{
  const GLfloat *norm;
  va_list list;
  va_start(list, vert);
  norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  Shader::use(vert, norm);
  
  // attribute �ϐ� pv, nv ��z��ϐ����瓾�邱�Ƃ�������
  glEnableVertexAttribArray(pvLoc);
  glEnableVertexAttribArray(nvLoc);
  
  // attribute �ϐ� pv, nv �Ɣz��ϐ� vert, norm �����т���
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);
  glVertexAttribPointer(nvLoc, 3, GL_FLOAT, GL_FALSE, 0, norm);
  
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

void SimpleShader::loadMatrix(const Matrix &mp, const Matrix &mw)
{
  loadModelViewPerspectiveMatrix(mp * mw);
  loadModelViewMatrix(mw);
  loadNormalMatrix(mw.normal());
}
