#ifndef __GG_SIMPLESHADER_H__
#define __GG_SIMPLESHADER_H__

/*
** �P���ȉA�e�t��
*/
#include "GgPointShader.h"

namespace gg
{

  class GgSimpleShader
  : public GgPointShader
  {
    // ���_
    GLint nvLoc;  // �@���� attribute �ϐ��̏ꏊ
    
  public:
    GgSimpleShader(void) {}
    GgSimpleShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgSimpleShader(void) {}
    
    // �V�F�[�_�̎g�p�J�n
    virtual void use(const GLfloat (*vert)[3], ...) const;
  };

}

#endif
