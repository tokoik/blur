#ifndef __GG_Pass2Shader_H__
#define __GG_Pass2Shader_H__

/*
** モーションブラー (Pass 2)
*/
#include "GgPointShader.h"

namespace gg
{
  class GgPass2Shader
    : public GgPointShader
  {
    // 乱数
    struct RnTable
    {
      static const int samples = 16;  // 乱数の数
      GLfloat rn[samples * 2];        // 乱数テーブル (vec2)
    } random;

    // 場所
    struct
    {
      GLint texture0;   // texture0 のサンプラの uniform 変数の場所
      GLint texture1;   // texture1 のサンプラの uniform 変数の場所
      GLint rn;         // 乱数テーブルの uniform 変数の場所
    } loc;

  public:
    
    // デストラクタ
    virtual ~GgPass2Shader(void) {}

    // コンストラクタ
    GgPass2Shader(void) {}
    GgPass2Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0,
      GLint nvarying = 0, const char **varyings = 0);
    GgPass2Shader(const GgPass2Shader &o)
      : GgPointShader(o), random(o.random), loc(o.loc) {}

    // 代入
    GgPass2Shader &operator=(const GgPass2Shader &o)
    {
      if (&o != this)
      {
        GgPointShader::operator=(o);
        loc = o.loc;
      }
      return *this;
    }

    // フィルタサイズ
    void size(GLfloat x, GLfloat y);

    // シェーダの使用開始
    virtual void use(GLuint vert, ...) const;
    
    // シェーダの使用終了
    virtual void unuse(void) const;
  };
};

#endif
