/*
** �^�C��
*/
#include <cstdarg>
#include <cmath>

#include "TileShader.h"

/*
** ��l�������� (Xorshift �@)
*/
static GLfloat xor128(void)
{ 
  static unsigned int x = 123456789;
  static unsigned int y = 362436069;
  static unsigned int z = 521288629;
  static unsigned int w = 88675123; 
  unsigned int t = x ^ x << 11;
  
  x = y;
  y = z;
  z = w;
  
  return (GLfloat)(w ^= w >> 19 ^ t ^ t >> 8); 
}

/*
** ���K�������� (Box Muller �@)
*/
static void boxmuller(GLfloat *r)
{
  float a = sqrt(-2.0f * log(((float)xor128() + 1.0f) / 4294967296.0f));
  float b = 6.2831853f * ((float)xor128() + 1.0f) / 4294967296.0f;
  
  r[0] = a * sin(b);
  r[1] = a * cos(b);
}

TileShader::TileShader(const char *vert, const char *frag, const char *geom) : SimpleShader(vert, frag, geom)
{
  // �e�N�X�`�����j�b�g�� uniform �ϐ��̏ꏊ
  texture0Loc = glGetUniformLocation(program, "texture0");
  texture1Loc = glGetUniformLocation(program, "texture1");
  
  // �V���h�E�}�b�s���O�p�� uniform �ϐ��̏ꏊ
  msLoc = glGetUniformLocation(program, "ms");
  
  // �r���[�|�[�g�� uniform �ϐ��̏ꏊ
  vpLoc = glGetUniformLocation(program, "vp");
  
  // ������ uniform �ϐ��̏ꏊ
  rnLoc = glGetUniformLocation(program, "rn");
  for (unsigned int i = 0; i < sizeof rn / sizeof rn[0]; i += 2) boxmuller(rn + i);
}

void TileShader::use(const GLfloat (*vert)[3], ...) const
{
  const GLfloat *norm;
  va_list list;
  va_start(list, vert);
  norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  SimpleShader::use(vert, norm);

  // �e�N�X�`�����j�b�g
  glUniform1i(texture0Loc, texture0);
  glUniform1i(texture1Loc, texture1);
  
  // ����
  glUniform2fv(rnLoc, sizeof rn / sizeof rn[0] / 2, rn);
  
  // �V���h�E�}�b�s���O�p�̕ϊ��s��
  glUniformMatrix4fv(msLoc, 1, GL_FALSE, ms);
  
  // �r���[�|�[�g
  glUniform4f(vpLoc, (GLfloat)vp[0],  (GLfloat)vp[1], (GLfloat)vp[2], (GLfloat)vp[3]);
}
