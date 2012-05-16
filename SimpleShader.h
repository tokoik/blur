#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H

/*
** 単純な陰影付け
*/
#include "Shader.h"
#include "Matrix.h"

class SimpleShader : public Shader
{
  // 光源
  GLfloat lpos[4];    // 光源位置
  GLint lposLoc;      // 光源位置の uniform 変数の場所
  GLfloat lamb[4];    // 光源強度の環境光成分
  GLint lambLoc;      // 光源強度の環境光成分の uniform 変数の場所
  GLfloat ldiff[4];   // 光源強度の拡散反射光成分
  GLint ldiffLoc;     // 光源強度の拡散反射光成分の uniform 変数の場所
  GLfloat lspec[4];   // 光源強度の鏡面反射光成分
  GLint lspecLoc;     // 光源強度の鏡面反射光成分の uniform 変数の場所
  GLfloat kamb[4];    // 環境光の反射係数
  GLint kambLoc;      // 環境光の反射係数の uniform 変数の場所
  
  // 材質
  GLfloat kdiff[4];   // 拡散反射係数
  GLint kdiffLoc;     // 拡散反射係数の uniform 変数の場所
  GLfloat kspec[4];   // 鏡面反射係数
  GLint kspecLoc;     // 鏡面反射係数の uniform 変数の場所
  GLfloat kshi;       // 輝き係数
  GLint kshiLoc;      // 輝き係数の uniform 変数の場所

  // 変換
  GLfloat mc[16];     // モデルビュー・投影変換行列
  GLint mcLoc;        // モデルビュー・投影変換行列の uniform 変数の場所
  GLfloat mw[16];     // モデルビュー変換行列
  GLint mwLoc;        // モデルビュー変換行列の uniform 変数の場所
  GLfloat mg[16];     // モデルビュー変換の法線変換行列
  GLint mgLoc;        // モデルビュー変換の法線変換行列の uniform 変数の場所
  
  // 属性
  GLint pvLoc;        // 頂点位置の attribute 変数の場所
  GLint nvLoc;        // 頂点法線の attribute 変数の場所
  
public:
  SimpleShader(void) {};
  SimpleShader(const char *vert, const char *frag, const char *geom = 0);
  virtual ~SimpleShader(void) {}
  virtual void use(const GLfloat (*vert)[3], ...) const;

  // 光源
  void setLightPosition(const GLfloat *pos) { setLightPosition(pos[0], pos[1], pos[2], pos[3]); }
  void setLightPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f) { lpos[0] = x; lpos[1] = y; lpos[2] = z; lpos[3] = w; }
  void setLightAmbient(const GLfloat *amb) { setLightAmbient(amb[0], amb[1], amb[2], amb[3]); }
  void setLightAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { lamb[0] = r; lamb[1] = g; lamb[2] = b; lamb[3] = a; }
  void setLightDiffuse(const GLfloat *diff) { setLightDiffuse(diff[0], diff[1], diff[2], diff[3]); }
  void setLightDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { ldiff[0] = r; ldiff[1] = g; ldiff[2] = b; ldiff[3] = a; }
  void setLightSpecular(const GLfloat *spec) { setLightSpecular(spec[0], spec[1], spec[2], spec[3]); }
  void setLightSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { lspec[0] = r; lspec[1] = g; lspec[2] = b; lspec[3] = a; }
  
  // 材質
  void setMaterialAmbient(const GLfloat *amb) { setMaterialAmbient(amb[0], amb[1], amb[2], amb[3]); }
  void setMaterialAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kamb[0] = r; kamb[1] = g; kamb[2] = b; kamb[3] = a; }
  void setMaterialDiffuse(const GLfloat *diff) { setMaterialDiffuse(diff[0], diff[1], diff[2], diff[3]); }
  void setMaterialDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kdiff[0] = r; kdiff[1] = g; kdiff[2] = b; kdiff[3] = a; }
  void setMaterialSpecular(const GLfloat *spec) { setMaterialSpecular(spec[0], spec[1], spec[2], spec[3]); }
  void setMaterialSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kspec[0] = r; kspec[1] = g; kspec[2] = b; kspec[3] = a; }
  void setMaterialShininess(GLfloat shi) { kshi = shi; }

  // 変換
  void loadModelViewPerspectiveMatrix(const GLfloat *m) { memcpy(mc, m, sizeof mc); }
  void loadModelViewPerspectiveMatrix(const Matrix &m) { memcpy(mc, m.get(), sizeof mc); }
  void loadModelViewMatrix(const GLfloat *m) { memcpy(mw, m, sizeof mw); }
  void loadModelViewMatrix(const Matrix &m) { memcpy(mw, m.get(), sizeof mw); }
  void loadNormalMatrix(const GLfloat *m) { memcpy(mg, m, sizeof mg); }
  void loadNormalMatrix(const Matrix &m) { memcpy(mg, m.get(), sizeof mg); }
  void loadMatrix(const Matrix &mp, const Matrix &mw);
};

#endif
