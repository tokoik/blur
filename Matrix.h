#ifndef MATRIX_H
#define MATRIX_H

/*
** ïœä∑çsóÒ
*/
#include <cstring>

#include "gg.h"

class Matrix {
  GLfloat array[16];
  void projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const;
  void multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const;
public:
  Matrix(void) {}
  Matrix(const GLfloat *a) { load(a); }
  Matrix(const Matrix &m) { load(m); }
  virtual ~Matrix(void) {}
  Matrix &load(const GLfloat *a) { memcpy(array, a, sizeof array); return *this; }
  Matrix &load(const Matrix &m) { return load(m.array); }
  Matrix &loadIdentity(void);
  Matrix &loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
  Matrix &loadTranslate(const GLfloat *t) { return loadTranslate(t[0], t[1], t[2], t[3]); }
  Matrix &loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
  Matrix &loadScale(const GLfloat *s) { return loadScale(s[0], s[1], s[2], s[3]); }
  Matrix &loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
  Matrix &loadRotate(const GLfloat *r) { return loadRotate(r[0], r[1], r[2], r[3]); }
  Matrix &loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
  Matrix &loadLookat(const GLfloat *e, const GLfloat *t, const GLfloat *u) { return loadLookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]); }
  Matrix &loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
  Matrix &loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
  Matrix &loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
  Matrix &loadTranspose(const Matrix &m);
  Matrix &loadInvert(const Matrix &m);
  Matrix &loadNormal(const Matrix &m);
  Matrix &multiply(const Matrix &m, const Matrix &n) { multiply(array, m.array, n.array); return *this; }
  Matrix &multiply(const Matrix &m, const GLfloat *a) { multiply(array, m.array, a); return *this; }
  Matrix &multiply(const GLfloat *a) { GLfloat t[16]; multiply(t, array, a); memcpy(array, t, sizeof array); return *this; }
  Matrix &multiply(const Matrix &m) { return multiply(m.array); }
  Matrix &operator=(const GLfloat *a) { return load(a); }
  Matrix &operator=(const Matrix &m) { return load(m); }
  Matrix operator*(const GLfloat *a) const { Matrix t; t.multiply(t.array, this->array, a); return t; }
  Matrix operator*(const Matrix &m) const { Matrix t; t.multiply(t.array, this->array, m.array); return t; }
  Matrix &operator*=(const GLfloat *a) { return multiply(a); }
  Matrix &operator*=(const Matrix &m) { return multiply(m); }
  Matrix &translate(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
  Matrix &translate(const GLfloat *t) { return translate(t[0], t[1], t[2], t[3]); }
  Matrix &scale(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
  Matrix &scale(const GLfloat *s) { return scale(s[0], s[1], s[2], s[3]); }
  Matrix &rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a);
  Matrix &rotate(const GLfloat *r) { return rotate(r[0], r[1], r[2], r[3]); }
  Matrix &lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz);
  Matrix &lookat(const GLfloat *e, const GLfloat *t, const GLfloat *u) { return lookat(e[0], e[1], e[2], t[0], t[1], t[2], u[0], u[1], u[2]); }
  Matrix &orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
  Matrix &frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
  Matrix &perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
  Matrix transpose(void) const { Matrix t; return t.loadTranspose(*this); }
  Matrix invert(void) const { Matrix t; return t.loadInvert(*this); }
  Matrix normal(void) const { Matrix t; return t.loadNormal(*this); }
  void projection(GLfloat *c, const GLfloat *v) const { projection(c, array, v); }
  const GLfloat *get(void) const { return array; }
};

#endif
