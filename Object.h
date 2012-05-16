#ifndef OBJECT_H
#define OBJECT_H

/*
** 三角形の形状データ
*/
#include "Shape.h"

class Object : public Shape {
  int nv, nf;                 // 頂点の数，面の数
  const GLfloat (*vert)[3];   // 頂点の位置
  const GLfloat (*norm)[3];   // 頂点の法線ベクトル
  const GLuint (*face)[3];    // 面データ（頂点のインデックス）
  void init(void);            // 初期化
  void copy(const Object &);  // メモリのコピー
  void free(void);            // メモリの解放
protected:
  void entry(int nv, int nf, const GLfloat (*vert)[3], const GLfloat (*norm)[3], const GLuint (*face)[3]);
public:
  Object(void) { init(); }
  Object(const Object &o) : Shape(o) { copy(o); }
  Object &operator=(const Object &o) { if (this != &o) copy(o); return *this; }
  virtual ~Object(void) { free(); }
  virtual void draw(void) const;
};

#endif
