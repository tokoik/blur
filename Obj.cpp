/*
** 三角形分割した Alias OBJ 形式ファイル
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <float.h>

#include "Obj.h"

/*
** ファイルの読み込み
*/
bool Obj::load(const char *name, bool normalize)
{
  // ファイルの読み込み
  std::ifstream file(name, std::ios::binary);
  if (file.fail()) {
    std::cerr << "Can't open file: " << name << std::endl;
    return false;
  }

  // データの数と座標値の最小値・最大値を調べる
  char buf[1024];
  int nv, nf;
  nv = nf = 0;
  float xmin, xmax, ymin, ymax, zmin, zmax;
  xmax = ymax = zmax = -(xmin = ymin = zmin = FLT_MAX);
  while (file.getline(buf, sizeof buf)) {
    if (buf[0] == 'v' && buf[1] == ' ') {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      if (x < xmin) xmin = x;
      if (x > xmax) xmax = x;
      if (y < ymin) ymin = y;
      if (y > ymax) ymax = y;
      if (z < zmin) zmin = z;
      if (z > zmax) zmax = z;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ') {
      ++nf;
    }
  }

  // メモリの確保
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  GLfloat (*fnorm)[3] = 0;
  GLuint (*face)[3] = 0;
  try {
    vert = new GLfloat[nv][3];
    norm = new GLfloat[nv][3];
    fnorm = new GLfloat[nf][3];
    face = new GLuint[nf][3];
  }
  catch (std::bad_alloc e) {
    delete[] vert;
    delete[] norm;
    delete[] fnorm;
    delete[] face;
    throw e;
  }

  // 正規化
  GLfloat scale, cx, cy, cz;
   if (normalize) {
    float sx = xmax - xmin;
    float sy = ymax - ymin;
    float sz = zmax - zmin;
    scale = sx;
    if (sy > scale) scale = sy;
    if (sz > scale) scale = sz;
    scale = (scale != 0.0f) ? 1.0f / scale : 1.0f;
    cx = (xmax + xmin) * 0.5f;
    cy = (ymax + ymin) * 0.5f;
    cz = (zmax + zmin) * 0.5f;
  }    
  else {
    scale = 1.0f;
    cx = cy = cz = 0.0f;
  }

  // ファイルの巻き戻し
  file.clear();
  file.seekg(0L, std::ios::beg);

  // データの読み込み
  nv = nf = 0;
  while (file.getline(buf, sizeof buf)) {
    if (buf[0] == 'v' && buf[1] == ' ') {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      vert[nv][0] = (x - cx) * scale;
      vert[nv][1] = (y - cy) * scale;
      vert[nv][2] = (z - cz) * scale;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ') {
      if (sscanf(buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3) {
        if (sscanf(buf + 2, "%d//%*d %d//%*d %d//%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3) {
          sscanf(buf + 2, "%d %d %d", face[nf], face[nf] + 1, face[nf] + 2);
        }
      }
      --face[nf][0];
      --face[nf][1];
      --face[nf][2];
      ++nf;
    }
  }

  // 面法線ベクトルの算出
  for (int i = 0; i < nf; ++i) {
    GLfloat dx1 = vert[face[i][1]][0] - vert[face[i][0]][0];
    GLfloat dy1 = vert[face[i][1]][1] - vert[face[i][0]][1];
    GLfloat dz1 = vert[face[i][1]][2] - vert[face[i][0]][2];
    GLfloat dx2 = vert[face[i][2]][0] - vert[face[i][0]][0];
    GLfloat dy2 = vert[face[i][2]][1] - vert[face[i][0]][1];
    GLfloat dz2 = vert[face[i][2]][2] - vert[face[i][0]][2];

    // 外積
    fnorm[i][0] = dy1 * dz2 - dz1 * dy2;
    fnorm[i][1] = dz1 * dx2 - dx1 * dz2;
    fnorm[i][2] = dx1 * dy2 - dy1 * dx2;
  }

  // 頂点の法線ベクトルの算出
  for (int i = 0; i < nv; ++i) {
    norm[i][0] = norm[i][1] = norm[i][2] = 0.0f;
  }
  for (int i = 0; i < nf; ++i) {
    norm[face[i][0]][0] += fnorm[i][0];
    norm[face[i][0]][1] += fnorm[i][1];
    norm[face[i][0]][2] += fnorm[i][2];

    norm[face[i][1]][0] += fnorm[i][0];
    norm[face[i][1]][1] += fnorm[i][1];
    norm[face[i][1]][2] += fnorm[i][2];

    norm[face[i][2]][0] += fnorm[i][0];
    norm[face[i][2]][1] += fnorm[i][1];
    norm[face[i][2]][2] += fnorm[i][2];
  }

  // 頂点の法線ベクトルの正規化
  for (int i = 0; i < nv; ++i) {
    GLfloat a = sqrt(norm[i][0] * norm[i][0]
                 + norm[i][1] * norm[i][1]
                 + norm[i][2] * norm[i][2]);

    if (a != 0.0) {
      norm[i][0] /= a;
      norm[i][1] /= a;
      norm[i][2] /= a;
    }
  }

  // 面の法線ベクトルは頂点の法線ベクトルの算出にしか使わないので解放
  delete[] fnorm;

  // データの登録
  entry(nv, nf, vert, norm, face);

  return true;
}
