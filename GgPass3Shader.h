#ifndef __GG_Pass3Shader_H__
#define __GG_Pass3Shader_H__

/*
** モーションブラー (Pass 3)
*/
#include "GgPointShader.h"

namespace gg
{
  class GgPass3Shader
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
    virtual ~GgPass3Shader(void) {}

    // コンストラクタ
    GgPass3Shader(void) {}
    GgPass3Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0,
      GLint nvarying = 0, const char **varyings = 0);
    GgPass3Shader(const GgPass3Shader &o)
      : GgPointShader(o), random(o.random), loc(o.loc) {}

    // 代入
    GgPass3Shader &operator=(const GgPass3Shader &o)
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
