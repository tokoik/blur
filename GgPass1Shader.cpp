/*
** ���[�V�����u���[ (Pass 1)
*/
#include <cstdarg>

#include "GgPass1Shader.h"

const char *gg::GgPass1Shader::varyings[] = { "p1" };

void gg::GgPass1Shader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);
  
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgSimpleShader::use(vert, norm);
  
  // �t�B�[�h�o�b�N�̃^�[�Q�b�g�ƂȂ�o�b�t�@�I�u�W�F�N�g��I������
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, b.fb[b.select].buf());
  
  // Transform Feedback �J�n
  glBeginTransformFeedback(GL_TRIANGLES);
}

void gg::GgPass1Shader::unuse(void) const
{
  // Transform Feedback �I��
  glEndTransformFeedback();
  
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgSimpleShader::unuse();
}

void gg::GgPass1Shader::copyBuffer(GLuint num, GLuint buf)
{
  // �t�B�[�h�o�b�N�o�b�t�@�p�̒��_�o�b�t�@�I�u�W�F�N�g�̃������̊m��
  b.fb[b.select].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[1 - b.select].load(GL_ARRAY_BUFFER, num, 0, GL_DYNAMIC_COPY);
  b.fb[b.select].copy(buf);
}
