#ifndef __GG_POINTSHADER_H__
#define __GG_POINTSHADER_H__

/*
** 点を球として表示する
*/
#include "gg.h"

namespace gg
{

  class GgPointShader
  : public GgShader
  {
    // 頂点
    GLint pvLoc;        // 位置の attribute 変数の場所
    
    // 変換
    GLfloat mc[16];     // モデルビュー・投影変換行列
    GLint mcLoc;        // モデルビュー・投影変換行列の uniform 変数の場所
    
  public:
    GgPointShader(void) {}
    GgPointShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_POINTS, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgPointShader(void) {}
    
    // シェーダの使用開始
    virtual void use(const GLfloat (*vert)[3], ...) const;
    
    // 変換
    void loadModelViewProjectionMatrix(const GgMatrix &m) { memcpy(mc, m.get(), sizeof mc); }
    void loadModelViewProjectionMatrix(const GLfloat *m) { memcpy(mc, m, sizeof mc); }
  };

}

#endif
