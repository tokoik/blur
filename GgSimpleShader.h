#ifndef __GG_SIMPLESHADER_H__
#define __GG_SIMPLESHADER_H__

/*
** 単純な陰影付け
*/
#include "GgPointShader.h"

namespace gg
{

  class GgSimpleShader
  : public GgPointShader
  {
    // 頂点
    GLint nvLoc;  // 法線の attribute 変数の場所
    
  public:
    GgSimpleShader(void) {}
    GgSimpleShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgSimpleShader(void) {}
    
    // シェーダの使用開始
    virtual void use(const GLfloat (*vert)[3], ...) const;
  };

}

#endif
