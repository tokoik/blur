/*
** ���[�V�����u���[
*/
#include <cstdarg>

#include "GgBlurShader.h"

const char *gg::GgBlurShader::varyings[] = { "p1" };

gg::GgBlurShader::GgBlurShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, GLint vertices)
  : GgSimpleShader(vert, frag, geom, input, output, vertices, 1, varyings)
{
  // �v���O������
  GLuint program = get();
  
  // �t�B�[�h�o�b�N�o�b�t�@����̓ǂݏo���p�� attribute �ϐ��̏ꏊ
  loc.p0 = glGetAttribLocation(program, "p0");
  
  // �ŏ��Ɏg�p����o�b�t�@
  b.buffer = 0;
}

void gg::GgBlurShader::use(GLuint vert, ...) const
{
  va_list list;
  va_start(list, vert);
  const GLuint norm = va_arg(list, GLuint);
  va_end(list);
  
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgSimpleShader::use(vert, norm);
  
  // �t�B�[�h�o�b�N�̃\�[�X�ƂȂ�o�b�t�@�I�u�W�F�N�g��I������
  glBindBuffer(GL_ARRAY_BUFFER, b.fb[b.buffer].buf());
  ggError("fb0");
  
  // attribute �ϐ� p0 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ�L���ɂ���
  glEnableVertexAttribArray(loc.p0);
  ggError("p0");
  
  // attribute �ϐ� nv �ƃo�b�t�@�I�u�W�F�N�g�����т���
  glVertexAttribPointer(loc.p0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  ggError("fb0a");
  
  // �t�B�[�h�o�b�N�̃^�[�Q�b�g�ƂȂ�o�b�t�@�I�u�W�F�N�g��I������
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, b.fb[1 - b.buffer].buf());
  ggError("fb1");
  
  // Transform Feedback �J�n
  glBeginTransformFeedback(GL_TRIANGLES);
  ggError("begin");
}

void gg::GgBlurShader::unuse(void) const
{
  // Transform Feedback �I��
  glEndTransformFeedback();
  
  // attribute �ϐ� p0 ���o�b�t�@�I�u�W�F�N�g���瓾�邱�Ƃ𖳌��ɂ���
  glDisableVertexAttribArray(loc.p0);
  
  // ���N���X�̃V�F�[�_�̐ݒ���Ăяo��
  GgSimpleShader::unuse();
}

void gg::GgBlurShader::copyBuffer(GLuint num, GLuint buf)
{
  // �t�B�[�h�o�b�N�o�b�t�@�p�̒��_�o�b�t�@�I�u�W�F�N�g�̃������̊m��
  b.fb[b.buffer].load(GL_ARRAY_BUFFER, num, 0);
  b.fb[1 - b.buffer].load(GL_ARRAY_BUFFER, num, 0);
  b.fb[b.buffer].copy(buf);
}