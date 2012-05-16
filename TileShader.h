#ifndef TILESHADER_H
#define TILESHADER_H

/*
** タイル
*/
#include "SimpleShader.h"
#include "Matrix.h"

#define SAMPLES 16

class TileShader : public SimpleShader
{
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
  GLfloat rn[SAMPLES * 2];  // 乱数
  GLint rnLoc;              // 乱数の uniform 変数の場所
  
  // テクスチャユニットの選択
  void active(GLuint u, GLuint t) const { glActiveTexture(GL_TEXTURE0 + u); glBindTexture(GL_TEXTURE_2D, t); }
  
public:
  TileShader(void) {};
  TileShader(const char *vert, const char *frag, const char *geom = 0);
  virtual ~TileShader(void) {}
  virtual void use(const GLfloat (*vert)[3], ...) const;

  // テクスチャユニット
  void setTexture0(GLuint unit, GLuint tex) { active(texture0 = unit, tex); }
  void setTexture1(GLuint unit, GLuint tex) { active(texture1 = unit, tex); }
  
  // シャドウマッピング用の変換行列
  void loadShadowMatrix(const GLfloat *m) { memcpy(ms, m, sizeof ms); }
  void loadShadowMatrix(const Matrix &m) { loadShadowMatrix(m.get()); }
  
  // ビューポート
  void saveViewport(void) { glGetIntegerv(GL_VIEWPORT, vp); }
  void restoreViewport(void) { glViewport(vp[0], vp[1], vp[2], vp[3]); }
};

#endif
