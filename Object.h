#ifndef OBJECT_H
#define OBJECT_H

/*
** �O�p�`�̌`��f�[�^
*/
#include "Shape.h"

class Object : public Shape {
  int nv, nf;                 // ���_�̐��C�ʂ̐�
  const GLfloat (*vert)[3];   // ���_�̈ʒu
  const GLfloat (*norm)[3];   // ���_�̖@���x�N�g��
  const GLuint (*face)[3];    // �ʃf�[�^�i���_�̃C���f�b�N�X�j
  void init(void);            // ������
  void copy(const Object &);  // �������̃R�s�[
  void free(void);            // �������̉��
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
