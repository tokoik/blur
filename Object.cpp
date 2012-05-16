/*
** �O�p�`�������� Alias OBJ �`���t�@�C���̓ǂݍ���
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <float.h>

#include "Object.h"

/*
** �I�u�W�F�N�g�̏�����
*/
void Object::init(void)
{
  nv = nf = 0;
  vert = norm = 0;
  face = 0;
}

/*
** �I�u�W�F�N�g�̃R�s�[
*/
void Object::copy(const Object &o)
{
  if (o.nv > 0 && o.nf > 0) {

    // �������̊m��
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

    // �f�[�^�̃R�s�[
    memcpy(vert, o.vert, sizeof(GLfloat) * 3 * o.nv);
    memcpy(norm, o.norm, sizeof(GLfloat) * 3 * o.nv);
    memcpy(face, o.face, sizeof(GLuint) * 3 * o.nf);

    // �f�[�^�̓o�^
    entry(o.nv, o.nf, vert, norm, face);
  }
}

/*
** �������̉��
*/
void Object::free(void)
{
  delete[] vert;
  delete[] norm;
  delete[] face;
}

/*
** �f�[�^�̓o�^
*/
void Object::entry(int nv, int nf, const GLfloat (*vert)[3], const GLfloat (*norm)[3], const GLuint (*face)[3])
{
  // �R�s�[��̃����������
  free();
  
  // �f�[�^�̓o�^
  this->nv = nv;
  this->nf = nf;
  this->vert = vert;
  this->norm = norm;
  this->face = face;
}

/*
** �}�`�̕\��
*/
void Object::draw(void) const
{
  // �V�F�[�_�v���O�����̑I��
  getShader()->use(vert, norm);
  
  // �J�����O�ݒ�
  glCullFace(getCullFace());
  
  // ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��
  glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, face);
}
