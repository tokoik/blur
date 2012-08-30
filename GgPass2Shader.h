#ifndef __GG_Pass2Shader_H__
#define __GG_Pass2Shader_H__

/*
** ���[�V�����u���[ (Pass 2)
*/
#include "gg.h"

namespace gg
{
  class GgPass2Shader
    : public GgShader
  {
    // �ꏊ
    struct
    {
      GLint p0;         // �ȑO�̃X�N���[����̒��_�ʒu
      GLint p1;         // ���݂̃X�N���[����̒��_�ʒu
    } loc;

  public:
    
    // �f�X�g���N�^
    virtual ~GgPass2Shader(void) {}
    
    // �R���X�g���N�^
    GgPass2Shader(void) {}
    GgPass2Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0);
    GgPass2Shader(const GgPass2Shader &o)
      : GgShader(o), loc(o.loc) {}
    
    // ���
    GgPass2Shader &operator=(const GgPass2Shader &o)
    {
      if (&o != this)
      {
        GgShader::operator=(o);
        loc = o.loc;
      }
      return *this;
    }

    // �V�F�[�_�̎g�p�J�n
    virtual void use(GLuint vert, ...) const;
    
    // �V�F�[�_�̎g�p�I��
    virtual void unuse(void) const;
  };
}

#endif
