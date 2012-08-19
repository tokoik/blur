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
}

void gg::GgSimpleShader::use(const GLfloat (*vert)[3], ...) const
{
  va_list list;
  va_start(list, vert);
  const GLfloat *norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  GgPointShader::use(vert);
  
  // attribute �ϐ� nv ��z��ϐ����瓾�邱�Ƃ�������
  glEnableVertexAttribArray(nvLoc);
  
  // attribute �ϐ� nv �Ɣz��ϐ� norm �����т���
  glVertexAttribPointer(nvLoc, 3, GL_FLOAT, GL_FALSE, 0, norm);
}
