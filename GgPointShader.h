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
    void loadModelViewProjectionMatrix(const GgMatrix &m) { memcpy(mc, m.get(), sizeof mc); }
    GLfloat mw[16];     // モデルビュー変換行列
    GLint mwLoc;        // モデルビュー変換行列の uniform 変数の場所
    void loadModelViewMatrix(const GgMatrix &m) { memcpy(mw, m.get(), sizeof mw); }

  public:

    // デストラクタ
    virtual ~GgPointShader(void) {}

    // コンストラクタ
    GgPointShader(void) {}
    GgPointShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_POINTS, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);

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
