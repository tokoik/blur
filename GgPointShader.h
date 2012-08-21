#ifndef __GG_POINTSHADER_H__
#define __GG_POINTSHADER_H__

/*
** �_�����Ƃ��ĕ\������
*/
#include "gg.h"

namespace gg
{

  class GgPointShader
  : public GgShader
  {
    // ���_
    GLint pvLoc;        // �ʒu�� attribute �ϐ��̏ꏊ
    
    // �ϊ�
    GLfloat mc[16];     // ���f���r���[�E���e�ϊ��s��
    GLint mcLoc;        // ���f���r���[�E���e�ϊ��s��� uniform �ϐ��̏ꏊ
    
  public:
    GgPointShader(void) {}
    GgPointShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_POINTS, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgPointShader(void) {}
    
    // �V�F�[�_�̎g�p�J�n
    virtual void use(const GLfloat (*vert)[3], ...) const;
    
    // �ϊ�
    void loadModelViewProjectionMatrix(const GgMatrix &m) { memcpy(mc, m.get(), sizeof mc); }
    void loadModelViewProjectionMatrix(const GLfloat *m) { memcpy(mc, m, sizeof mc); }
  };

}

#endif
