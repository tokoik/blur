#ifndef __GG_Pass1Shader_H__
#define __GG_Pass1Shader_H__

/*
** ���[�V�����u���[ (Pass 1)
*/
#include "GgSimpleShader.h"

namespace gg
{
  class GgPass1Shader
    : public GgSimpleShader
  {
    // feedback buffer �� varying �ϐ���
    static const char *varyings[];
    
    // �o�b�t�@�I�u�W�F�N�g
    struct
    {
      // feedback buffer �Ɏg���o�b�t�@�I�u�W�F�N�g
      GgBuffer<GLfloat[3]> fb[2];

      // feedback buffer �̑I��
      unsigned int buffer;
    } b;
    
    // �ꏊ
    struct
    {
      GLint p0;         // �t�B�[�h�o�b�N�o�b�t�@����̓ǂݏo���p�� attribute �ϐ��̏ꏊ
    } loc;
    
  public:
    
    // �f�X�g���N�^
    virtual ~GgPass1Shader(void) {}
    
    // �R���X�g���N�^
    GgPass1Shader(void) {}
    GgPass1Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0);
    GgPass1Shader(const GgPass1Shader &o)
      : GgSimpleShader(o), b(o.b), loc(o.loc) {}
    
    // ���
    GgPass1Shader &operator=(const GgPass1Shader &o)
    {
      if (&o != this)
      {
        GgSimpleShader::operator=(o);
        b = o.b;
        loc = o.loc;
      }
      return *this;
    }

    // �V�F�[�_�̎g�p�J�n
    virtual void use(GLuint vert, ...) const;
    
    // �V�F�[�_�̎g�p�I��
    virtual void unuse(void) const;
    
    // �o�b�t�@�I�u�W�F�N�g�̕���
    void copyBuffer(GLuint num, GLuint buf);
    
    // �t�B�[�h�o�b�N�o�b�t�@�I�u�W�F�N�g�̑I��
    void selectBuffer(unsigned int frame)
    {
      b.buffer = frame & 1;
    }
    
    // �t�B�[�h�o�b�N�o�b�t�@�I�u�W�F�N�g�̑I��
    void swapBuffers(void)
    {
      b.buffer = 1 - b.buffer;
    }
  };
}

#endif
