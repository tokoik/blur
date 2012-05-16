/*
** �l�p�`
*/
#include "Square.h"

void Square::resize(GLfloat w, GLfloat h)
{
  static const GLfloat v[][2] = {
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
  };

  for (int i = 0; i < 4; ++i) {

    vert[i][0] = v[i][0] * w;
    vert[i][1] = v[i][1] * h;
    vert[i][2] = 0.0f;

    norm[i][0] = 0.0f;
    norm[i][1] = 0.0f;
    norm[i][2] = 1.0f;
  }
}

void Square::draw(void) const
{
  // �V�F�[�_�v���O�����̑I��
  getShader()->use(vert, norm);
  
  // �J�����O�ݒ�
  glCullFace(getCullFace());
  
  // ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
