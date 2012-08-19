#ifndef __GG_TILESHADER_H__
#define __GG_TILESHADER_H__

/*
** �^�C��
*/
#include "GgSimpleShader.h"

namespace gg
{

  class GgTileShader
  : public GgSimpleShader
  {
    // �f�荞�݂̃T���v����
    static const int samples = 16;

    // �e�N�X�`�����j�b�g
    GLuint texture0;          // 0 �Ԗڂ̃e�N�X�`�����j�b�g
    GLint texture0Loc;        // 0 �Ԗڂ̃e�N�X�`�����j�b�g�� uniform �ϐ��̏ꏊ
    GLuint texture1;          // 1 �Ԗڂ̃e�N�X�`�����j�b�g
    GLint texture1Loc;        // 1 �Ԗڂ̃e�N�X�`�����j�b�g�� uniform �ϐ��̏ꏊ

    // �V���h�E�}�b�s���O�p�̕ϊ��s��
    GLfloat ms[16];           // �V���h�E�}�b�s���O�p�̕ϊ��s��
    GLint msLoc;              // �V���h�E�}�b�s���O�p�̕ϊ��s��� uniform �ϐ��̏ꏊ

    // �r���|�[�g
    GLint vp[4];              // �r���[�|�[�g
    GLint vpLoc;              // �r���[�|�[�g�� uniform �ϐ��̏ꏊ

    // ����
    GLfloat rn[samples * 2];  // ����
    GLint rnLoc;              // ������ uniform �ϐ��̏ꏊ

    // �e�N�X�`�����j�b�g�̑I��
    void active(GLuint u, GLuint t) const { glActiveTexture(GL_TEXTURE0 + u); glBindTexture(GL_TEXTURE_2D, t); }

  public:
    GgTileShader(void) {}
    GgTileShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, int vertices = 0,
      int nvarying = 0, const char **varyings = 0);
    virtual ~GgTileShader(void) {}
    
    // �V�F�[�_�̎g�p�J�n
    virtual void use(const GLfloat (*vert)[3], ...) const;

    // �e�N�X�`�����j�b�g
    void setTexture0(GLuint unit, GLuint tex) { active(texture0 = unit, tex); }
    void setTexture1(GLuint unit, GLuint tex) { active(texture1 = unit, tex); }

    // �V���h�E�}�b�s���O�p�̕ϊ��s��
    void loadShadowMatrix(const GLfloat *m) { memcpy(ms, m, sizeof ms); }
    void loadShadowMatrix(const GgMatrix &m) { loadShadowMatrix(m.get()); }

    // �r���[�|�[�g
    void saveViewport(void) { glGetIntegerv(GL_VIEWPORT, vp); }
    void restoreViewport(void) { glViewport(vp[0], vp[1], vp[2], vp[3]); }
  };

}

#endif
