#ifndef __GG_POINTSHADER_H__
#define __GG_POINTSHADER_H__

/*
** ポイント
*/
#include "gg.h"

namespace gg
{

  class GgPointShader
    : public GgShader
  {
    // 変換
    struct
    {
      GLfloat c[16];    // モデルビュー・投影変換行列
      void loadModelViewProjectionMatrix(const GgMatrix &m)
      {
        for (int i = 0; i < 16; ++i) c[i] = m.get()[i];
      }
      GLfloat w[16];    // モデルビュー変換行列
      void loadModelViewMatrix(const GgMatrix &m)
      {
        for (int i = 0; i < 16; ++i) w[i] = m.get()[i];
      }
    } m;

    // 場所
    struct
    {
      GLint pv;         // 位置の attribute 変数の場所
      GLint mc;         // モデルビュー・投影変換行列の uniform 変数の場所
      GLint mw;         // モデルビュー変換行列の uniform 変数の場所
    } loc;

  public:

    // デストラクタ
    virtual ~GgPointShader(void) {}

    // コンストラクタ
    GgPointShader(void) {}
    GgPointShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_POINTS, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0,
      GLint nvarying = 0, const char **varyings = 0);
    GgPointShader(const GgPointShader &o)
      : GgShader(o), m(o.m), loc(o.loc) {}

    // 代入
    GgPointShader &operator=(const GgPointShader &o)
    {
      if (&o != this)
      {
        GgShader::operator=(o);
        m = o.m;
        loc = o.loc;
      }
      return *this;
    }

    // シェーダの使用開始
    virtual void use(GLuint vert, ...) const;

    // シェーダの使用終了
    virtual void unuse(void) const;

    // 変換
    virtual void loadMatrix(const GgMatrix &mp, const GgMatrix &mw);
    virtual void loadMatrix(const GLfloat *mp, const GLfloat *mw);
  };

}

#endif
