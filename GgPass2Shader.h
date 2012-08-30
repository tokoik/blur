#ifndef __GG_Pass2Shader_H__
#define __GG_Pass2Shader_H__

/*
** モーションブラー (Pass 2)
*/
#include "gg.h"

namespace gg
{
  class GgPass2Shader
    : public GgShader
  {
    // 場所
    struct
    {
      GLint p0;         // 以前のスクリーン上の頂点位置
      GLint p1;         // 現在のスクリーン上の頂点位置
    } loc;

  public:
    
    // デストラクタ
    virtual ~GgPass2Shader(void) {}
    
    // コンストラクタ
    GgPass2Shader(void) {}
    GgPass2Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0);
    GgPass2Shader(const GgPass2Shader &o)
      : GgShader(o), loc(o.loc) {}
    
    // 代入
    GgPass2Shader &operator=(const GgPass2Shader &o)
    {
      if (&o != this)
      {
        GgShader::operator=(o);
        loc = o.loc;
      }
      return *this;
    }

    // シェーダの使用開始
    virtual void use(GLuint vert, ...) const;
    
    // シェーダの使用終了
    virtual void unuse(void) const;
  };
}

#endif
