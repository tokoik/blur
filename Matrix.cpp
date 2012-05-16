/*
** �ϊ��s��
*/
#include <cmath>

#include "Matrix.h"

/*
** �s��ƃx�N�g���̐� c �� a �~ b
*/
void Matrix::projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 4; ++i) {
    c[i] = a[0 + i] * b[0] + a[4 + i] * b[1] + a[8 + i] * b[2] + a[12 + i] * b[3];
  }
}

/*
** �s��̐� c �� a �~ b
*/
void Matrix::multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 16; ++i) {
    int j = i & 3, k = i & ~3;
    
    c[i] = a[0 + j] * b[k + 0] + a[4 + j] * b[k + 1] + a[8 + j] * b[k + 2] + a[12 + j] * b[k + 3];
  }
}

/*
** �P�ʍs���ݒ肷��
*/
Matrix &Matrix::loadIdentity(void)
{
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[ 0] = array[ 5] = array[10] = array[15] = 1.0f;

  return *this;
}

/*
** ���s�ړ��ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  if (w != 0.0f) {
    x /= w;
    y /= w;
    z /= w;
  }
  array[12] = x;
  array[13] = y;
  array[14] = z;
  array[ 1] = array[ 2] = array[ 3] =
  array[ 4] = array[ 6] = array[ 7] =
  array[ 8] = array[ 9] = array[11] = 0.0f;
  array[ 0] = array[ 5] = array[10] = array[15] = 1.0f;
  
  return *this;
}

/*
** �g��k���ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  array[ 0] = x;
  array[ 5] = y;
  array[10] = z;
  array[15] = w;
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  
  return *this;
}

/*
** ��]�ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  GLfloat d = sqrt(x * x + y * y + z * z);
  
  if (d > 0.0f) {
    GLfloat l  = x / d, m  = y / d, n  = z / d;
    GLfloat l2 = l * l, m2 = m * m, n2 = n * n;
    GLfloat lm = l * m, mn = m * n, nl = n * l;
    GLfloat c = cos(a), c1 = 1.0f - c;
    GLfloat s = sin(a);
    
    array[ 0] = (1.0f - l2) * c + l2;
    array[ 1] = lm * c1 + n * s;
    array[ 2] = nl * c1 - m * s;
    array[ 3] = 0.0f;
    
    array[ 4] = lm * c1 - n * s;
    array[ 5] = (1.0f - m2) * c + m2;
    array[ 6] = mn * c1 + l * s;
    array[ 7] = 0.0f;
    
    array[ 8] = nl * c1 + m * s;
    array[ 9] = mn * c1 - l * s;
    array[10] = (1.0f - n2) * c + n2;
    array[11] = 0.0f;
    
    array[12] = 0.0f;
    array[13] = 0.0f;
    array[14] = 0.0f;
    array[15] = 1.0f;
  }

  return *this;
}

/*
** �]�u�s���ݒ肷��
*/
Matrix &Matrix::loadTranspose(const Matrix &m)
{
  array[ 0] = m.array[ 0];
  array[ 1] = m.array[ 4];
  array[ 2] = m.array[ 8];
  array[ 3] = m.array[12];
  array[ 4] = m.array[ 1];
  array[ 5] = m.array[ 5];
  array[ 6] = m.array[ 9];
  array[ 7] = m.array[13];
  array[ 8] = m.array[ 2];
  array[ 9] = m.array[ 6];
  array[10] = m.array[10];
  array[11] = m.array[14];
  array[12] = m.array[ 3];
  array[13] = m.array[ 7];
  array[14] = m.array[11];
  array[15] = m.array[15];
  
  return *this;
}

/*
** �t�s���ݒ肷��
*/
Matrix &Matrix::loadInvert(const Matrix &m)
{
  GLfloat lu[20], *plu[4];
  const GLfloat *marray = m.array;
  
  // j �s�̗v�f�̒l�̐�Βl�̍ő�l�� plu[j][4] �ɋ��߂�
  for (int j = 0; j < 4; ++j) {
    GLfloat max = fabs(*(plu[j] = lu + 5 * j) = *(marray++));
    
    for (int i = 0; ++i < 4;) {
      GLfloat a = fabs(plu[j][i] = *(marray++));
      if (a > max) max = a;
    }
    if (max == 0.0f) return *this;
    plu[j][4] = 1.0f / max;
  }
  
  // �s�{�b�g���l������ LU ����
  for (int j = 0; j < 4; ++j) {
    GLfloat max = fabs(plu[j][j] * plu[j][4]);
    int i = j;
    
    for (int k = j; ++k < 4;) {
      GLfloat a = fabs(plu[k][j] * plu[k][4]);
      if (a > max) {
        max = a;
        i = k;
      }
    }
    if (i > j) {
      GLfloat *t = plu[j];
      plu[j] = plu[i];
      plu[i] = t;
    }
    if (plu[j][j] == 0.0f) return *this;
    for (int k = j; ++k < 4;) {
      plu[k][j] /= plu[j][j];
      for (int i = j; ++i < 4;) {
        plu[k][i] -= plu[j][i] * plu[k][j];
      }
    }
  }
  
  // LU ��������t�s������߂�
  for (int k = 0; k < 4; ++k) {
    // array �ɒP�ʍs���ݒ肷��
    for (int i = 0; i < 4; ++i) {
      array[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0f : 0.0f;
    }
    // lu ����t�s������߂�
    for (int i = 0; i < 4; ++i) {
      for (int j = i; ++j < 4;) {
        array[j * 4 + k] -= array[i * 4 + k] * plu[j][i];
      }
    }
    for (int i = 4; --i >= 0;){
      for (int j = i; ++j < 4;) {
        array[i * 4 + k] -= plu[i][j] * array[j * 4 + k];
      }
      array[i * 4 + k] /= plu[i][i];
    }
  }
  
  return *this;
}

/*
** �@���ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadNormal(const Matrix &m)
{
  array[ 0] = m.array[ 5] * m.array[10] - m.array[ 6] * m.array[ 9];
  array[ 1] = m.array[ 6] * m.array[ 8] - m.array[ 4] * m.array[10];
  array[ 2] = m.array[ 4] * m.array[ 9] - m.array[ 5] * m.array[ 8];
  array[ 4] = m.array[ 9] * m.array[ 2] - m.array[10] * m.array[ 1];
  array[ 5] = m.array[10] * m.array[ 0] - m.array[ 8] * m.array[ 2];
  array[ 6] = m.array[ 8] * m.array[ 1] - m.array[ 9] * m.array[ 0];
  array[ 8] = m.array[ 1] * m.array[ 6] - m.array[ 2] * m.array[ 5];
  array[ 9] = m.array[ 2] * m.array[ 4] - m.array[ 0] * m.array[ 6];
  array[10] = m.array[ 0] * m.array[ 5] - m.array[ 1] * m.array[ 4];
  array[ 3] = array[ 7] = array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[15] = 1.0f;

  return *this;
}

/*
** ����ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  GLfloat l;

  /* z �� = e - t */
  tx = ex - tx;
  ty = ey - ty;
  tz = ez - tz;
  l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 2] = tx / l;
  array[ 6] = ty / l;
  array[10] = tz / l;

  /* x �� = u x z �� */
  tx = uy * array[10] - uz * array[ 6];
  ty = uz * array[ 2] - ux * array[10];
  tz = ux * array[ 6] - uy * array[ 2];
	l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 0] = tx / l;
  array[ 4] = ty / l;
  array[ 8] = tz / l;

  /* y �� = z �� x x �� */
  array[ 1] = array[ 6] * array[ 8] - array[10] * array[ 4];
  array[ 5] = array[10] * array[ 0] - array[ 2] * array[ 8];
  array[ 9] = array[ 2] * array[ 4] - array[ 6] * array[ 0];

  /* ���s�ړ� */
  array[12] = -(ex * array[ 0] + ey * array[ 4] + ez * array[ 8]);
  array[13] = -(ex * array[ 1] + ey * array[ 5] + ez * array[ 9]);
  array[14] = -(ex * array[ 2] + ey * array[ 6] + ez * array[10]);

  /* �c�� */
  array[ 3] = array[ 7] = array[11] = 0.0f;
  array[15] = 1.0f;

  return *this;
}

/*
** ���s���e�ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
    
    array[ 0] =  2.0f / dx;
    array[ 5] =  2.0f / dy;
    array[10] = -2.0f / dz;
    array[12] = -(right + left) / dx;
    array[13] = -(top + bottom) / dy;
    array[14] = -(zFar + zNear) / dz;
    array[15] =  1.0f;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] = array[11] = 0.0f;
  }
  
  return *this;
}

/*
** �������e�ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
    
    array[ 0] =  2.0f * zNear / dx;
    array[ 5] =  2.0f * zNear / dy;
    array[ 8] =  (right + left) / dx;
    array[ 9] =  (top + bottom) / dy;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[12] = array[13] = array[15] = 0.0f;
  }
    
  return *this;
}

/*
** ��p���瓧�����e�ϊ��s���ݒ肷��
*/
Matrix &Matrix::loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  GLfloat dz = zFar - zNear;
  
  if (dz != 0.0f) {
    GLfloat f = 1.0f / tan(fovy * 0.5f);
    
    array[ 0] = f / aspect;
    array[ 5] = f;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] =
    array[12] = array[13] = array[15] = 0.0f;
  }

  return *this;
}

/*
** ���s�ړ��ϊ��s����悶��
*/
Matrix &Matrix::translate(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  Matrix m;

  m.loadTranslate(x, y, z, w);
  multiply(m);

  return *this;
}

/*
** �g��k���ϊ��s����悶��
*/
Matrix &Matrix::scale(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  Matrix m;

  m.loadScale(x, y, z, w);
  multiply(m);

  return *this;
}

/*
** ��]�s��ϊ����悶��
*/
Matrix &Matrix::rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  Matrix m;

  m.loadRotate(x, y, z, a);
  multiply(m);

  return *this;
}

/*
** ���_�̈ړ�
*/
Matrix &Matrix::lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  Matrix m;

  m.loadLookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
  multiply(m);

  return *this;
}

/*
** ���s���e�ϊ��s��
*/
Matrix &Matrix::orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  Matrix m;

  m.loadOrthogonal(left, right, bottom, top, zNear, zFar);
  multiply(m);

  return *this;
}

/*
** �������e�ϊ��s��
*/
Matrix &Matrix::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  Matrix m;

  m.loadFrustum(left, right, bottom, top, zNear, zFar);
  multiply(m);
    
  return *this;
}

/*
** ��p���瓧�����e�ϊ��s��
*/
Matrix &Matrix::perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  Matrix m;

  m.loadPerspective(fovy, aspect, zNear, zFar);
  multiply(m);

  return *this;
}
