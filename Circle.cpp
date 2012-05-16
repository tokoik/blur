/*
** ë»â~å`
*/
#include <new>
#include <cmath>

#include "Circle.h"

void Circle::resize(GLfloat w, GLfloat h, int slices)
{
  // ÉÅÉÇÉäÇÃämï€
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  GLuint (*face)[3] = 0;
  try {
    vert = new GLfloat[slices][3];
    norm = new GLfloat[slices][3];
    face = new GLuint[slices - 2][3];
  }
  catch (std::bad_alloc e) {
    delete[] vert;
    delete[] norm;
    delete[] face;
    throw e;
  }

  for (int i = 0; i < slices; ++i) {
    float t = 6.2831853f * (float)i / (float)slices;

    vert[i][0] = cos(t) * w * 0.5f;
    vert[i][1] = sin(t) * h * 0.5f;
    vert[i][2] = 0.0f;

    norm[i][0] = 0.0f;
    norm[i][1] = 0.0f;
    norm[i][2] = 1.0f;
  }

  for (int i = 0; i < slices - 2; ++i) {
    face[i][0] = 0;
    face[i][1] = i + 1;
    face[i][2] = i + 2;
  }

  entry(slices, slices - 2, vert, norm, face);
}
