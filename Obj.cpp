/*
** �O�p�`�������� Alias OBJ �`���t�@�C��
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <float.h>

#include "Obj.h"

/*
** �t�@�C���̓ǂݍ���
*/
bool Obj::load(const char *name, bool normalize)
{
  // �t�@�C���̓ǂݍ���
  std::ifstream file(name, std::ios::binary);
  if (file.fail()) {
    std::cerr << "Can't open file: " << name << std::endl;
    return false;
  }

  // �f�[�^�̐��ƍ��W�l�̍ŏ��l�E�ő�l�𒲂ׂ�
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

  // �������̊m��
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

  // ���K��
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

  // �t�@�C���̊����߂�
  file.clear();
  file.seekg(0L, std::ios::beg);

  // �f�[�^�̓ǂݍ���
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

  // �ʖ@���x�N�g���̎Z�o
  for (int i = 0; i < nf; ++i) {
    GLfloat dx1 = vert[face[i][1]][0] - vert[face[i][0]][0];
    GLfloat dy1 = vert[face[i][1]][1] - vert[face[i][0]][1];
    GLfloat dz1 = vert[face[i][1]][2] - vert[face[i][0]][2];
    GLfloat dx2 = vert[face[i][2]][0] - vert[face[i][0]][0];
    GLfloat dy2 = vert[face[i][2]][1] - vert[face[i][0]][1];
    GLfloat dz2 = vert[face[i][2]][2] - vert[face[i][0]][2];

    // �O��
    fnorm[i][0] = dy1 * dz2 - dz1 * dy2;
    fnorm[i][1] = dz1 * dx2 - dx1 * dz2;
    fnorm[i][2] = dx1 * dy2 - dy1 * dx2;
  }

  // ���_�̖@���x�N�g���̎Z�o
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

  // ���_�̖@���x�N�g���̐��K��
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

  // �ʂ̖@���x�N�g���͒��_�̖@���x�N�g���̎Z�o�ɂ����g��Ȃ��̂ŉ��
  delete[] fnorm;

  // �f�[�^�̓o�^
  entry(nv, nf, vert, norm, face);

  return true;
}
