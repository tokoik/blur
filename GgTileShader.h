#ifndef __GG_TILESHADER_H__
#define __GG_TILESHADER_H__

/*
** タイル
*/
#include "GgSimpleShader.h"

namespace gg
{

  class GgTileShader
  : public GgSimpleShader
  {
    // 映り込みのサンプル数
    static const int samples = 16;

    // テクスチャユニット
    GLuint texture0;          // 0 番目のテクスチャユニット
    GLint texture0Loc;        // 0 番目のテクスチャユニットの uniform 変数の場所
    GLuint texture1;          // 1 番目のテクスチャユニット
    GLint texture1Loc;        // 1 番目のテクスチャユニットの uniform 変数の場所

    // シャドウマッピング用の変換行列
    GLfloat ms[16];           // シャドウマッピング用の変換行列
    GLint msLoc;              // シャドウマッピング用の変換行列の uniform 変数の場所

    // ビュポート
    GLint vp[4];              // ビューポート
    GLint vpLoc;              // ビューポートの uniform 変数の場所

    // 乱数
    GLfloat rn[samples * 2];  // 乱数
    GLint rnLoc;              // 乱数の uniform 変数の場所

    // テクスチャユニットの選択
    void active(GLuint u, GLuint t) const { glActiveTexture(GL_TEXTURE0 + u); glBindTexture(GL_TEXTURE_2D, t); }

  public:
    GgTileShader(void) {}
    GgTileShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgTileShader(void) {}
    
    // シェーダの使用開始
    virtual void use(const GLfloat (*vert)[3], ...) const;

    // テクスチャユニット
    void setTexture0(GLuint unit, GLuint tex) { active(texture0 = unit, tex); }
    void setTexture1(GLuint unit, GLuint tex) { active(texture1 = unit, tex); }

    // シャドウマッピング用の変換行列
    void loadShadowMatrix(const GLfloat *m) { memcpy(ms, m, sizeof ms); }
    void loadShadowMatrix(const GgMatrix &m) { loadShadowMatrix(m.get()); }

    // ビューポート
    void saveViewport(void) { glGetIntegerv(GL_VIEWPORT, vp); }
    void restoreViewport(void) { glViewport(vp[0], vp[1], vp[2], vp[3]); }
  };

}

#endif
