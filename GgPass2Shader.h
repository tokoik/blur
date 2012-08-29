#ifndef __GG_Pass2Shader_H__
#define __GG_Pass2Shader_H__

/*
** ���[�V�����u���[ (Pass 2)
*/
#include "GgPointShader.h"

namespace gg
{
  class GgPass2Shader
    : public GgPointShader
  {
    // ����
    struct RnTable
    {
      static const int samples = 16;  // �����̐�
      GLfloat rn[samples * 2];        // �����e�[�u�� (vec2)
    } random;

    // �ꏊ
    struct
    {
      GLint texture0;   // texture0 �̃T���v���� uniform �ϐ��̏ꏊ
      GLint texture1;   // texture1 �̃T���v���� uniform �ϐ��̏ꏊ
      GLint rn;         // �����e�[�u���� uniform �ϐ��̏ꏊ
    } loc;

  public:
    
    // �f�X�g���N�^
    virtual ~GgPass2Shader(void) {}

    // �R���X�g���N�^
    GgPass2Shader(void) {}
    GgPass2Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0,
      GLint nvarying = 0, const char **varyings = 0);
    GgPass2Shader(const GgPass2Shader &o)
      : GgPointShader(o), random(o.random), loc(o.loc) {}

    // ���
    GgPass2Shader &operator=(const GgPass2Shader &o)
    {
      if (&o != this)
      {
        GgPointShader::operator=(o);
        loc = o.loc;
      }
      return *this;
    }

    // �t�B���^�T�C�Y
    void size(GLfloat x, GLfloat y);

    // �V�F�[�_�̎g�p�J�n
    virtual void use(GLuint vert, ...) const;
    
    // �V�F�[�_�̎g�p�I��
    virtual void unuse(void) const;
  };
};

#endif
