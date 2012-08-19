/*
** ゲームグラフィックス特論用補助プログラム
**

Copyright (c) 2011, 2012 Kohe Tokoi. All Rights Reserved.
 
Permission is hereby granted, free of charge,  to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction,  including without limitation the rights 
to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell 
copies or substantial portions of the Software.
 
The above  copyright notice  and this permission notice  shall be included in 
all copies or substantial portions of the Software.
 
THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR 
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY, 
FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL 
KOHE TOKOI  BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY,  WHETHER IN 
AN ACTION  OF CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM,  OUT OF  OR  IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
**
*/

#ifndef __GG_H__
#define __GG_H__

#include <cstring>
#include <cmath>

#if defined(WIN32)
#  pragma warning(disable:4996)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  pragma comment(lib, "glew32.lib")
#  include "glew.h"
#  include "wglew.h"
#  include "glut.h"
#elif defined(X11)
#  define GL_GLEXT_PROTOTYPES
#  define GLX_GLXEXT_PROTOTYPES
#  include <GL/glut.h>
#elif defined(__APPLE__)
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
// GL_ARB_texture_float
#  define GL_RGBA32F GL_RGBA32F_ARB
#  define GL_RGB32F GL_RGB32F_ARB
#  define GL_ALPHA32F GL_ALPHA32F_ARB
#  define GL_INTENSITY32F GL_INTENSITY32F_ARB
#  define GL_LUMINANCE32F GL_LUMINANCE32F_ARB
#  define GL_LUMINANCE_ALPHA32F GL_LUMINANCE_ALPHA32F_ARB
#  define GL_RGBA16F GL_RGBA16F_ARB
#  define GL_RGB16F GL_RGB16F_ARB
#  define GL_ALPHA16F GL_ALPHA16F_ARB
#  define GL_INTENSITY16F GL_INTENSITY16F_ARB
#  define GL_LUMINANCE16F GL_LUMINANCE16F_ARB
#  define GL_LUMINANCE_ALPHA16F GL_LUMINANCE_ALPHA16F_ARB
// GL_EXT_transform_feedback
#  define glBindBufferRange glBindBufferRangeEXT
#  define glBindBufferOffset glBindBufferOffsetEXT
#  define glBindBufferBase glBindBufferBaseEXT
#  define glBeginTransformFeedback glBeginTransformFeedbackEXT
#  define glEndTransformFeedback glEndTransformFeedbackEXT
#  define glTransformFeedbackVaryings glTransformFeedbackVaryingsEXT
#  define glGetTransformFeedbackVarying glGetTransformFeedbackVaryingEXT
#  define glGetIntegerIndexedv glGetIntegerIndexedvEXT
#  define glGetBooleanIndexedv glGetBooleanIndexedvEXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER GL_TRANSFORM_FEEDBACK_BUFFER_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_START GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT
#  define GL_INTERLEAVED_ATTRIBS GL_INTERLEAVED_ATTRIBS_EXT
#  define GL_SEPARATE_ATTRIBS GL_SEPARATE_ATTRIBS_EXT
#  define GL_PRIMITIVES_GENERATED GL_PRIMITIVES_GENERATED_EXT
#  define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT
#  define GL_RASTERIZER_DISCARD GL_RASTERIZER_DISCARD_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYINGS GL_TRANSFORM_FEEDBACK_VARYINGS_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_MODE GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT
// GL_APPLE_vertex_array_object
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#  define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#  define glIsVertexArray glIsVertexArrayAPPLE
#  define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_APPLE
#else
#  error "This platform is not supported."
#endif

namespace gg
{
  /*
  ** ゲームグラフィックス特論の都合にもとづく初期化
  **
  **     OpenGL のレンダリングコンテキストを作成後に実行する
  */
  extern void ggInit(void);
  
  /*
  ** OpenGL のエラーチェック
  **
  **     OpenGL の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **     msg はメッセージに乗じる文字列
  */
  extern void ggError(const char *msg = 0);
  
  /*
  ** FBO のエラーチェック
  **
  **     FBO の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **     msg はメッセージに乗じる文字列
  */
  extern void ggFBOError(const char *msg = 0);
  
  /*
  ** シェーダーソースファイルの読み込み
  */
  extern GLuint loadShader(
    const char *vert,                   // バーテックスシェーダのソースファイル名
    const char *frag = 0,               // フラグメントシェーダのソースファイル名（0 なら不使用）
    const char *geom = 0,               // ジオメトリシェーダのソースファイル名（0 なら不使用）
    GLenum input = GL_TRIANGLES,        // ジオメトリシェーダの入力プリミティブ
    GLenum output = GL_TRIANGLE_STRIP,  // ジオメトリシェーダの出力プリミティブ
    int vertices = 0,                   // ジオメトリシェーダの出力頂点数
    int nvarying = 0,                   // フィードバックする varying 変数の数（0 なら不使用）
    const char **varyings = 0           // フィードバックする varying 変数のリスト
  );
  
  /*
  ** テクスチャマッピング用の RAW 画像ファイルの読み込み
  */
  extern void loadImage(const char *name, int width, int height, GLenum format);
  
  /*
  ** 高さマップ用の RAW 画像ファイルの読み込んで法線マップを作成する
  */
  extern void loadHeight(const char *name, int width, int height, float nz);
  
  /*
  ** 基底クラス
  */
  class Gg
  {
  protected:
    
    // デストラクタ
    ~Gg(void) {}
    
  public:
    
    // コンストラクタ
    Gg(void) {}
  };
  
  /*
  ** 変換行列
  */
  class GgMatrix
  : public Gg
  {
    // 変換行列の要素
    GLfloat array[16];
    
    // 行列 a とベクトル b の積をベクトル c に代入する
    void projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const;
    
    // 行列 a と行列 b の積を行列 c に代入する
    void multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const;
    
  public:
    
    // デストラクタ
    virtual ~GgMatrix(void) {}
    
    // コンストラクタ
    GgMatrix(void) {}
    GgMatrix(const GLfloat *a)
    {
      load(a);
    }
    GgMatrix(const GgMatrix &m)
    {
      load(m);
    }
    
    // 演算子
    GgMatrix &multiply(const GgMatrix &m, const GgMatrix &n)
    {
      multiply(array, m.array, n.array);
      return *this;
    }
    GgMatrix &multiply(const GgMatrix &m, const GLfloat *a)
    {
      multiply(array, m.array, a);
      return *this;
    }
    GgMatrix &multiply(const GLfloat *a)
    {
      GLfloat t[16]; multiply(t, array, a);
      memcpy(array, t, sizeof array); return *this;
    }
    GgMatrix &multiply(const GgMatrix &m)
    {
      return multiply(m.array);
    }
    GgMatrix &operator=(const GLfloat *a)
    {
      return load(a);
    }
    GgMatrix &operator=(const GgMatrix &m)
    {
      return load(m);
    }
    GgMatrix operator*(const GLfloat *a) const
    {
      GgMatrix t;
      t.multiply(t.array, this->array, a);
      return t;
    }
    GgMatrix operator*(const GgMatrix &m) const
    {
      GgMatrix t;
      t.multiply(t.array, this->array, m.array);
      return t;
    }
    GgMatrix &operator*=(const GLfloat *a)
    {
      return multiply(a);
    }
    GgMatrix &operator*=(const GgMatrix &m)
    {
      return multiply(m);
    }
    
    // 変換行列の読み込み
    GgMatrix &load(const GLfloat *a)
    {
      memcpy(array, a, sizeof array);
      return *this;
    }
    GgMatrix &load(const GgMatrix &m)
    {
      return load(m.array);
    }
    
    // 単位行列を設定する
    GgMatrix &loadIdentity(void);
    
    // 平行移動の変換行列を設定する
    GgMatrix &loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
    GgMatrix &loadTranslate(const GLfloat *t)
    {
      return loadTranslate(t[0], t[1], t[2], t[3]);
    }
    
    // 拡大縮小の変換行列を設定する
    GgMatrix &loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
    GgMatrix &loadScale(const GLfloat *s)
    {
      return loadScale(s[0], s[1], s[2], s[3]);
    }
    
    // 回転の変換行列を設定する
    GgMatrix &loadRotateX(GLfloat a);
    GgMatrix &loadRotateY(GLfloat a);
    GgMatrix &loadRotateZ(GLfloat a);
    GgMatrix &loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
    GgMatrix &loadRotate(const GLfloat *r)
    {
      return loadRotate(r[0], r[1], r[2], r[3]);
    }
    
    // 視野変換行列を設定する
    GgMatrix &loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
    GgMatrix &loadLookat(const GLfloat *e, const GLfloat *t, const GLfloat *u)
    {
      return loadLookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]);
    }
    
    // 直交投影変換行列を設定する
    GgMatrix &loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    
    // 透視透視投影変換行列を設定する
    GgMatrix &loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    
    // 画角を指定して透視投影変換行列を設定する
    GgMatrix &loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
    
    // 転置行列を設定する
    GgMatrix &loadTranspose(const GgMatrix &m);
    
    // 逆行列を設定する
    GgMatrix &loadInvert(const GgMatrix &m);
    
    // 法線変換行列を設定する
    GgMatrix &loadNormal(const GgMatrix &m);
    
    // 平行移動変換を乗じる
    GgMatrix &translate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f)
    {
      GgMatrix m; m.loadTranslate(x, y, z, w); multiply(m);
      return *this;
    }
    GgMatrix &translate(const GLfloat *t)
    {
      return translate(t[0], t[1], t[2], t[3]);
    }
    
    // 拡大縮小変換を乗じる
    GgMatrix &scale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f)
    {
      GgMatrix m;
      m.loadScale(x, y, z, w);
      multiply(m);
      return *this;
    }
    GgMatrix &scale(const GLfloat *s)
    {
      return scale(s[0], s[1], s[2], s[3]);
    }
    
    // 回転変換を乗じる
    GgMatrix &rotateX(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateX(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotateY(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateY(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotateZ(GLfloat a)
    {
      GgMatrix m;
      m.loadRotateZ(a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
    {
      GgMatrix m;
      m.loadRotate(x, y, z, a);
      multiply(m);
      return *this;
    }
    GgMatrix &rotate(const GLfloat *r)
    {
      return rotate(r[0], r[1], r[2], r[3]);
    }
    
    // 視野変換を乗じる
    GgMatrix &lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
    GgMatrix &lookat(const GLfloat *e, const GLfloat *t, const GLfloat *u)
    {
      return lookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]);
    }
    
    // 直交投影変換を乗じる
    GgMatrix &orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    
    // 透視投影変換を乗じる
    GgMatrix &frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    
    // 画角を指定して透視投影変換を乗じる
    GgMatrix &perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
    
    // 転置行列を得る
    GgMatrix transpose(void) const
    {
      GgMatrix t;
      return t.loadTranspose(*this);
    }
    
    // 逆行列を得る
    GgMatrix invert(void) const
    {
      GgMatrix t;
      return t.loadInvert(*this);
    }
    
    // 法線変換行列を得る
    GgMatrix normal(void) const
    {
      GgMatrix t;
      return t.loadNormal(*this);
    }
    
    // ベクトルに対して投影変換を行う
    void projection(GLfloat *c, const GLfloat *v) const
    {
      projection(c, array, v);
    }
    
    // 変換行列を取り出す
    const GLfloat *get(void) const
    {
      return array;
    }
  };
  
  /*
  ** 四元数
  */
  class GgQuaternion
  : public Gg
  {
    // 四元数の要素
    GLfloat array[4];
    
    // 四元数 p と四元数 q の和を四元数 r に求める
    void add(GLfloat *r, const GLfloat *p, const GLfloat *q) const;
    
    // 四元数 p と四元数 q の差を四元数 r に求める
    void subtract(GLfloat *r, const GLfloat *p, const GLfloat *q) const;
    
    // 四元数 p と四元数 q の積を四元数 r に求める
    void multiply(GLfloat *r, const GLfloat *p, const GLfloat *q) const;
    
    // 四元数 q が表す回転の変換行列を m に求める
    void toMatrix(GLfloat *m, const GLfloat *q) const;
    
    // 回転の変換行列 m が表す四元数を q に求める
    void toQuaternion(GLfloat *q, const GLfloat *m) const;
    
    // 球面線形補間 q と r を t で補間した四元数を p に求める
    void slerp(GLfloat *p, const GLfloat *q, const GLfloat *r, GLfloat t) const;
    
  public:
    
    // デストラクタ
    virtual ~GgQuaternion(void) {}
    
    // コンストラクタ
    GgQuaternion(void) {}
    GgQuaternion(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
      load(x, y, z, w);
    }
    GgQuaternion(const GLfloat *a)
    {
      load(a);
    }
    GgQuaternion(const GgQuaternion &q)
    {
      load(q);
    }
    
    // 演算子
    GgQuaternion &operator=(const GLfloat *a)
    {
      return load(a);
    }
    GgQuaternion &operator=(const GgQuaternion &q)
    {
      return load(q);
    }
    GgQuaternion operator+(const GLfloat *a) const
    {
      GgQuaternion t;
      t.add(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator+(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.add(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator+=(const GLfloat *a)
    {
      return add(a);
    }
    GgQuaternion &operator+=(const GgQuaternion &q)
    {
      return add(q);
    }
    GgQuaternion operator-(const GLfloat *a) const
    {
      GgQuaternion t;
      t.subtract(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator-(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.subtract(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator-=(const GLfloat *a)
    {
      return subtract(a);
    }
    GgQuaternion &operator-=(const GgQuaternion &q)
    {
      return subtract(q);
    }
    GgQuaternion operator*(const GLfloat *a) const
    {
      GgQuaternion t;
      t.multiply(t.array, this->array, a);
      return t;
    }
    GgQuaternion operator*(const GgQuaternion &q) const
    {
      GgQuaternion t;
      t.multiply(t.array, this->array, q.array);
      return t;
    }
    GgQuaternion &operator*=(const GLfloat *a)
    {
      return multiply(a);
    }
    GgQuaternion &operator*=(const GgQuaternion &q)
    {
      return multiply(q);
    }
    
    // 四元数を設定する
    GgQuaternion &load(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
      array[0] = x;
      array[1] = y;
      array[2] = z;
      array[3] = w;
      return *this;
    }
    GgQuaternion &load(const GLfloat *a)
    {
      return load(a[0], a[1], a[2], a[3]);
    }
    GgQuaternion &load(const GgQuaternion &q)
    {
      return load(q.array);
    }
    
    // 回転の変換行列 m を表す四元数を設定する
    GgQuaternion &loadMatrix(const GLfloat *m)
    {
      toQuaternion(array, m); return *this;
    }
    
    // 単位元を設定する
    GgQuaternion &loadIdentity(void)
    {
      return load(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // (x, y, z) を軸として角度 a 回転する資源数を設定する
    GgQuaternion &loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
    GgQuaternion &loadRotate(const GLfloat *v, GLfloat a)
    {
      return loadRotate(v[0], v[1], v[2], a);
    }
    
    // オイラー角 (h, p, r) で与えられた回転を表す四元数を設定する
    GgQuaternion &loadEuler(GLfloat h, GLfloat p, GLfloat r);
    GgQuaternion &loadEuler(const GLfloat *e)
    {
      return loadEuler(e[0], e[1], e[2]);
    }
    
    // 四元数の和を求める
    GgQuaternion &add(const GgQuaternion &p, const GgQuaternion &q)
    {
      add(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &add(const GgQuaternion &q, const GLfloat *a)
    {
      add(array, q.array, a);
      return *this;
    }
    GgQuaternion &add(const GLfloat *a)
    {
      GLfloat t[4];
      add(t, array, a);
      return load(t);
    }
    GgQuaternion &add(const GgQuaternion &q)
    {
      return add(q.array);
    }
    
    // 四元数の差を求める
    GgQuaternion &subtract(const GgQuaternion &p, const GgQuaternion &q)
    {
      subtract(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &subtract(const GgQuaternion &q, const GLfloat *a)
    {
      subtract(array, q.array, a);
      return *this;
    }
    GgQuaternion &subtract(const GLfloat *a)
    {
      GLfloat t[4];
      subtract(t, array, a);
      return load(t);
    }
    GgQuaternion &subtract(const GgQuaternion &q)
    {
      return subtract(q.array);
    }
    
    // 四元数の積を求める
    GgQuaternion &multiply(const GgQuaternion &p, const GgQuaternion &q)
    {
      multiply(array, p.array, q.array);
      return *this;
    }
    GgQuaternion &multiply(const GgQuaternion &q, const GLfloat *a)
    {
      multiply(array, q.array, a);
      return *this;
    }
    GgQuaternion &multiply(const GLfloat *a)
    {
      GLfloat t[4]; multiply(t, array, a);
      return load(t);
    }
    GgQuaternion &multiply(const GgQuaternion &q)
    {
      return multiply(q.array);
    }
    
    // 球面線形補間
    GgQuaternion &slerp(const GgQuaternion &q, const GgQuaternion &r, GLfloat t)
    {
      slerp(array, q.array, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(const GgQuaternion &q, const GLfloat *b, GLfloat t)
    {
      slerp(array, q.array, b, t);
      return *this;
    }
    GgQuaternion &slerp(const GLfloat *a, const GgQuaternion &r, GLfloat t)
    {
      slerp(array, a, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(const GLfloat *a, const GLfloat *b, GLfloat t)
    {
      slerp(array, a, b, t);
      return *this;
    }
    GgQuaternion &slerp(const GgQuaternion &r, GLfloat t)
    {
      slerp(array, array, r.array, t);
      return *this;
    }
    GgQuaternion &slerp(GLfloat *b, GLfloat t)
    {
      slerp(array, array, b, t);
      return *this;
    }
    
    // 四元数のノルムを求める
    GLfloat norm(void) const;
    
    // 共役四元数を求める
    GgQuaternion conjugate(void) const;
    
    // 四元数の逆を求める
    GgQuaternion invert(void) const;
    
    // 四元数を正規化する
    GgQuaternion normalize(void) const;
    
    // 四元数を取り出す
    const GLfloat *get(void) const
    {
      return array;
    }
    void get(GLfloat *a) const
    {
      a[0] = array[0];
      a[1] = array[1];
      a[2] = array[2];
      a[3] = array[3];
    }
    
    // 四元数が表す回転の行列を m に求める
    void getMatrix(GLfloat *m) const
    {
      toMatrix(m, array);
    }
  };
  
  /*
  ** 簡易トラックボール処理
  */
  class GgTrackball
  : public Gg
  {
    int cx, cy;       // ドラッグ開始位置
    bool drag;        // ドラッグ中か否か
    float sx, sy;     // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
    GgQuaternion cq;  // 回転の初期値 (四元数)
    GgQuaternion tq;  // ドラッグ中の回転 (四元数)
    GLfloat rt[16];   // 回転の変換行列
    
  public:
    
    // デストラクタ
    virtual ~GgTrackball(void) {}
    
    // コンストラクタ
    GgTrackball(void);
    
    // トラックボール処理の範囲指定
    //    ウィンドウのリサイズ時に呼び出す
    void region(int w, int h);
    
    // トラックボール処理の開始
    //    マウスのドラッグ開始時（マウスボタンを押したとき）に呼び出す
    void start(int x, int y);
    
    // 回転の変換行列の計算
    //    マウスのドラッグ中に呼び出す
    void motion(int x, int y);
    
    // トラックボール処理の停止
    //    マウスのドラッグ終了時（マウスボタンを離したとき）に呼び出す
    void stop(int x, int y);
    
    // 現在の回転の変換行列を取り出す
    const GLfloat *get(void) const
    {
      return rt;
    }
  };

  /*
  ** 属性データ
  **
  **     テクスチャとシェーダの基底クラス
  **     インスタンスは複数のオブジェクトから参照されることを想定する
  **     そのためこのクラスでは参照カウントを管理する
  */
  class GgAttribute
  : public Gg
  {
    // 参照カウント
    int count;
    
  protected:
    
    // 属性の結合
    template<class T> void attach(T *&target, T *instance)
    {
      target = instance;
      if (target) target->inc();
    }
    
    // 属性の解放
    template<class T> void detach(T *&target)
    {
      if (target && target->dec() <= 0) delete target;
    }
    
    // デストラクタ
    ~GgAttribute(void) {}
    
  public:
    
    // コンストラクタ
    GgAttribute(void)
    {
      count = 0;
    }
    
    // 参照カウンタの増減
    int inc(void)
    {
      return ++count;
    }
    int dec(void)
    {
      return --count;
    }
  };
  
  /*
  ** テクスチャ
  **
  **     拡散反射色テクスチャを読み込んでテクスチャオブジェクトを作成する
  */
  class GgTexture
  : public GgAttribute
  {
    // テクスチャ名
    GLuint texture;
    
    // コピーコンストラクタ禁止
    GgTexture(const GgTexture &o);

    // 代入演算子禁止
    GgTexture &operator=(const GgTexture &o);
    
  protected:
    
    // テクスチャオブジェクトを取り出す
    GLuint get(void) const
    {
      return texture;
    }
    
  public:
    
    // デストラクタ
    virtual ~GgTexture(void)
    {
      if (texture != 0)
      {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &texture);
      }
    }
    
    // コンストラクタ
    GgTexture(void)
    {
      glGenTextures(1, &texture);
    }
    GgTexture(
      const char *name,                   // 画像ファイル名（3/4 チャネルの RAW 画像）
      int width, int height,              // 画像の幅と高さ（2^n 画素）
      GLenum format = GL_RGB              // 読み込む画像の書式 (GL_RGB/GL_RGBA)
    )
    {
      GgTexture();
      load(name, width, height, format);
    }
    
    // 拡散反射色テクスチャを読み込む
    //     name: ファイル名, width, height: 幅と高さ (2^n), format: GL_RGB か GL_RGBA
    void load(const char *name, int width, int height, GLenum format = GL_RGB) const
    {
      loadImage(name, width, height, format);
    }
    
    // テクスチャオブジェクトを結合する
    //     このテクスチャを使用する際に呼び出す
    //     unit: 使用するテクスチャユニット番号（0～）
    void use(unsigned int unit = 0) const
    {
      glActiveTexture(GL_TEXTURE0 + unit);
      glBindTexture(GL_TEXTURE_2D, texture);
    }
    
    // テクスチャオブジェクトを解放する
    //    このテクスチャを使用しなくなったら呼び出す
    void unuse(void) const
    {
      glBindTexture(GL_TEXTURE_2D, 0);
      glActiveTexture(GL_TEXTURE0);
    }
  };
  
  /*
  ** 法線マップ
  **
  **     高さマップ（グレイスケール画像）を読み込んで法線マップを作成する
  */
  class GgNormalTexture
  : public GgTexture
  {
    // コピーコンストラクタ禁止
    GgNormalTexture(const GgNormalTexture &o);
    
    // 代入演算子禁止
    GgNormalTexture &operator=(const GgNormalTexture &o);
    
  public:
    
    // デストラクタ
    virtual ~GgNormalTexture(void) {}
    
    // コンストラクタ
    GgNormalTexture(void) {}
    GgNormalTexture(
      const char *name,                   // 画像ファイル名（1 チャネルの RAW 画像）
      int width, int height,              // 画像の幅と高さ（2^n 画素）
      float nz = 1.0f                     // 法線マップの z 成分の値
    )
    : GgTexture()
    {
      load(name, width, height, nz);
    }
    
    // 高さマップを読み込んで法線マップを作成する
    //    name: ファイル名, width, height: 幅と高さ (2^n), nz: 法線マップの z 成分の値
    void load(const char *name, int width, int height, float nz = 1.0f) const
    {
      loadHeight(name, width, height, nz);
    }
  };
  
  /*
  ** シェーダ
  **
  **     シェーダの基底クラス
  */
  class GgShader
  : public GgAttribute
  {
    // プログラム名
    GLuint program;

    // コピーコンストラクタ禁止
    GgShader(const GgShader &o);
    
    // 代入演算子禁止
    GgShader &operator=(const GgShader &o);
    
  public:

    // デストラクタ
    virtual ~GgShader(void)
    {
      if (program != 0)
      {
        glUseProgram(0);
        glDeleteProgram(program);
      }
    }
    
    // コンストラクタ
    GgShader(void)
    {
      program = 0;
    }
    GgShader(
      const char *vert,                   // バーテックスシェーダのソースファイル名
      const char *frag = 0,               // フラグメントシェーダのソースファイル名（0 なら不使用）
      const char *geom = 0,               // ジオメトリシェーダのソースファイル名（0 なら不使用）
      GLenum input = GL_TRIANGLES,        // ジオメトリシェーダの入力プリミティブ
      GLenum output = GL_TRIANGLE_STRIP,  // ジオメトリシェーダの出力プリミティブ
      int vertices = 0,                   // ジオメトリシェーダの出力頂点数
      int nvarying = 0,                   // フィードバックする varying 変数の数（0 なら不使用）
      const char **varyings = 0           // フィードバックする varying 変数のリスト
    )
    {
      load(vert, frag, geom, input, output, vertices, nvarying, varyings);
    }
    
    // シェーダのソースプログラムの読み込みとコンパイル・リンク
    void load(
      const char *vert,                   // バーテックスシェーダのソースファイル名
      const char *frag = 0,               // フラグメントシェーダのソースファイル名（0 なら不使用）
      const char *geom = 0,               // ジオメトリシェーダのソースファイル名（0 なら不使用）
      GLenum input = GL_TRIANGLES,        // ジオメトリシェーダの入力プリミティブ
      GLenum output = GL_TRIANGLE_STRIP,  // ジオメトリシェーダの出力プリミティブ
      int vertices = 0,                   // ジオメトリシェーダの出力頂点数
      int nvarying = 0,                   // フィードバックする varying 変数の数（0 なら不使用）
      const char **varyings = 0           // フィードバックする varying 変数のリスト
    )
    {
      program = loadShader(vert, frag, geom, input, output, vertices, nvarying, varyings);
    }
    
    // シェーダプログラムの使用開始
    virtual void use(const GLfloat (*vert)[3], ...) const
    {
      glUseProgram(program);
    }
    
    // シェーダプログラムの使用終了
    void unuse(void) const
    {
      glUseProgram(0);
    }
    
    // シェーダプログラム名を取り出す
    GLuint get(void) const
    {
      return program;
    }
  };
  
  /*
  ** 形状データ
  **
  **    形状データの基底クラス
  **    このクラスから使用するシェーダを参照する
  **    シェーダのインスタンスを結合したら参照カウントをインクリメントする
  **    シェーダのインスタンスの結合を解除したら参照カウントをデクリメントし，0 になったらシェーダを削除する
  */
  class GgShape
  : public Gg
  {
    // シェーダー
    GgShader *shader;
    
    // シェーダを結合する
    void bind(GgShader *s)
    {
      shader = s;
      if (shader) shader->inc();
    }
    
    // シェーダを解放する
    void unbind(void)
    {
      if (shader && shader->dec() == 0) delete shader;
    }
    
  public:
    
    // デストラクタ
    virtual ~GgShape(void)
    {
      unbind();
    }
    
    // コンストラクタ
    GgShape(void)
    {
      shader = 0;
    }
    GgShape(const GgShape &o)
    {
      bind(o.shader);
    }
    
    // 代入演算子
    GgShape &operator=(const GgShape &o)
    {
      if (this != &o) attachShader(o.shader);
      return *this;
    }
    
    // 形状データにシェーダのインスタンス s を結合する
    //    それまで結合されていたシェーダの参照カウントをデクリメントして 0 になったらそのシェーダを破棄する
    //    新しいシェーダ s を結合して s の参照カウントをインクリメントする
    void attachShader(GgShader *s)
    {
      unbind();
      bind(s);
    }
    
    // この形状データで使用しているシェーダを得る
    GgShader *getShader(void) const
    {
      return shader;
    }
    
    // この形状を描画する手続きをオーバーライドする
    virtual void draw(void) const = 0;
  };
  
  /*
  ** 点群
  */
  class GgPoints
  : public GgShape
  {
    // 頂点の数
    GLuint nv;
    
    // 頂点の位置
    const GLfloat (*vert)[3];
    
    // コピー
    void copy(const GgPoints &o);
    
  protected:
    
    // データを登録する
    void entry(GLuint nv, const GLfloat (*vert)[3])
    {
      this->nv = nv;
      this->vert = vert;
    }
    
    // 頂点の数を取り出す
    GLuint getNv(void) const
    {
      return nv;
    }
    
    // 頂点データを取り出す
    const GLfloat (*getVert(void) const)[3]
    {
      return vert;
    }
    
  public:
    
    // デストラクタ
    virtual ~GgPoints(void)
    {
      delete[] vert;
    }
    
    // コンストラクタ
    GgPoints(void)
    {
      entry(0, 0);
    }
    GgPoints(GLuint nv, const GLfloat (*vert)[3])
    {
      entry(nv, vert);
    }
    GgPoints(const GgPoints &o)
    : GgShape(o) 
    {
      copy(o);
    }
    
    // 代入演算子
    GgPoints &operator=(const GgPoints &o)
    {
      if (this != &o)
      {
        GgShape::operator=(o);
        copy(o);
      }
      return *this;
    }
    
    // 点群の描画
    virtual void draw(void) const;
  };
  
  /*
  ** メッシュ
  */
  class GgPolygon
  : public GgPoints
  {
    // 頂点の法線ベクトル
    const GLfloat (*norm)[3];
    
    // コピー
    void copy(const GgPolygon &o);
    
  protected:
    
    // データを登録する
    void entry(const GLfloat (*norm)[3])
    {
      this->norm = norm;
    }
    
    // 法線ベクトルを取り出す
    const GLfloat (*getNorm(void) const)[3]
    {
      return norm;
    }
    
  public:
    
    // デストラクタ
    virtual ~GgPolygon(void)
    {
      delete[] norm;
    }
    
    // コンストラクタ
    GgPolygon(void)
    {
      entry(0);
    }
    GgPolygon(GLuint nv, const GLfloat (*vert)[3], const GLfloat (*norm)[3])
    : GgPoints(nv, vert)
    {
      entry(norm);
    }
    GgPolygon(const GgPolygon &o)
    : GgPoints(o)
    {
      copy(o);
    }
    
    // 代入演算子
    GgPolygon &operator=(const GgPolygon &o)
    {
      if (this != &o)
      {
        GgPoints::operator=(o);
        copy(o);
      }
      return *this;
    }
    
    // オブジェクトの描画
    virtual void draw(void) const;
  };

  /*
  ** 三角形の形状データ
  */
  class GgObject
  : public GgPolygon
  {
    // 面の数
    GLuint nf;
    
    // 面データ（頂点のインデックス）
    const GLuint (*face)[3];
    
    // コピー
    void copy(const GgObject &o);
    
  protected:
    
    // データを登録する
    void entry(GLuint nf, const GLuint (*face)[3])
    {
      this->nf = nf;
      this->face = face;
    }
    
    // 面の数を取り出す
    GLuint getNf(void) const
    {
      return nf;
    }
    
    // 面データを取り出す
    const GLuint (*getFace(void) const)[3]
    {
      return face;
    }
    
  public:
    
    // デストラクタ
    virtual ~GgObject(void)
    {
      delete[] face;
    }
    
    // コンストラクタ
    GgObject(void)
    {
      entry(0, 0);
    }
    GgObject(GLuint nv, const GLfloat (*vert)[3], const GLfloat (*norm)[3], GLuint nf, const GLuint (*face)[3])
    : GgPolygon(nv, vert, norm)
    {
      entry(nf, face);
    }
    GgObject(const GgObject &o)
    : GgPolygon(o)
    {
      copy(o);
    }
    
    // 代入演算子
    GgObject &operator=(const GgObject &o)
    {
      if (this != &o)
      {
        GgPolygon::operator=(o);
        copy(o);
      }
      return *this;
    }
    
    // メッシュの描画
    virtual void draw(void) const;
  };

  /*
  ** 球状の点群
  */
  extern GgPoints *ggPointSphere(GLuint nv, GLfloat cx = 0.0f, GLfloat cy = 0.0f, GLfloat cz = 0.0f, GLfloat radius = 0.5f);
  
  /*
  ** 矩形
  */
  extern GgPolygon *ggRectangle(GLfloat width = 1.0f, GLfloat height = 1.0f);
  
  /*
  ** 楕円
  */
  extern GgPolygon *ggEllipse(GLfloat width = 1.0f, GLfloat height = 1.0f, GLuint slices = 16);
  
  /*
  ** 三角形分割された Alias OBJ ファイル
  */
  extern GgObject *ggObj(const char *name, bool normalize = false);
}

#endif
