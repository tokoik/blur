/*
** 三角形分割した Alias OBJ 形式ファイルの読み込み
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <float.h>

#include "Object.h"

/*
** オブジェクトの初期化
*/
void Object::init(void)
{
  nv = nf = 0;
  vert = norm = 0;
  face = 0;
}

/*
** オブジェクトのコピー
*/
void Object::copy(const Object &o)
{
  if (o.nv > 0 && o.nf > 0) {

    // メモリの確保
    GLfloat (*vert)[3] = 0;
    GLfloat (*norm)[3] = 0;
    GLuint (*face)[3] = 0;
    try {
      vert = new GLfloat[o.nv][3];
      norm = new GLfloat[o.nv][3];
      face = new GLuint[o.nf][3];
    }
    catch (std::bad_alloc e) {
      delete[] vert;
      delete[] norm;
      delete[] face;
      throw e;
    }

    // データのコピー
    memcpy(vert, o.vert, sizeof(GLfloat) * 3 * o.nv);
    memcpy(norm, o.norm, sizeof(GLfloat) * 3 * o.nv);
    memcpy(face, o.face, sizeof(GLuint) * 3 * o.nf);

    // データの登録
    entry(o.nv, o.nf, vert, norm, face);
  }
}

/*
** メモリの解放
*/
void Object::free(void)
{
  delete[] vert;
  delete[] norm;
  delete[] face;
}

/*
** データの登録
*/
void Object::entry(int nv, int nf, const GLfloat (*vert)[3], const GLfloat (*norm)[3], const GLuint (*face)[3])
{
  // コピー先のメモリを解放
  free();
  
  // データの登録
  this->nv = nv;
  this->nf = nf;
  this->vert = vert;
  this->norm = norm;
  this->face = face;
}

/*
** 図形の表示
*/
void Object::draw(void) const
{
  // シェーダプログラムの選択
  getShader()->use(vert, norm);
  
  // カリング設定
  glCullFace(getCullFace());
  
  // 頂点のインデックスの場所を指定して図形を描画する
  glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, face);
}
