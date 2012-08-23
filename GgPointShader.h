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
    void loadModelViewProjectionMatrix(const GgMatrix &m) { memcpy(mc, m.get(), sizeof mc); }
    GLfloat mw[16];     // ���f���r���[�ϊ��s��
    GLint mwLoc;        // ���f���r���[�ϊ��s��� uniform �ϐ��̏ꏊ
    void loadModelViewMatrix(const GgMatrix &m) { memcpy(mw, m.get(), sizeof mw); }

  public:

    // �f�X�g���N�^
    virtual ~GgPointShader(void) {}

    // �R���X�g���N�^
    GgPointShader(void) {}
    GgPointShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_POINTS, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);

    // �V�F�[�_�̎g�p�J�n
    virtual void use(GLuint vert, ...) const;

    // �V�F�[�_�̎g�p�I��
    virtual void unuse(void) const;

    // �ϊ�
    virtual void loadMatrix(const GgMatrix &mp, const GgMatrix &mw);
    virtual void loadMatrix(const GLfloat *mp, const GLfloat *mw);
  };

}

#endif
