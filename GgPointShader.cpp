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
}

void gg::GgPointShader::use(const GLfloat (*vert)[3], ...) const
{
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgShader::use(0);
  
  // attribute �ϐ� pv ��z��ϐ����瓾�邱�Ƃ�������
  glEnableVertexAttribArray(pvLoc);
  
  // attribute �ϐ� pv �Ɣz��ϐ� vert �����т���
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);

  // �ϊ��s���ݒ肷��
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
}
