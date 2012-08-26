#ifndef __GG_SIMPLESHADER_H__
#define __GG_SIMPLESHADER_H__

/*
** �P���ȉA�e�t��
*/
#include "GgPointShader.h"

namespace gg
{
  class GgSimpleShader
    : public GgPointShader
  {
    // ����
    struct
    {
      GLfloat pos[4];   // �����ʒu
      GLfloat amb[4];   // �������x�̊�������
      GLfloat diff[4];  // �������x�̊g�U���ˌ�����
      GLfloat spec[4];  // �������x�̋��ʔ��ˌ�����
    } l;

    // �ގ�
    struct
    {
      GLfloat amb[4];   // �����̔��ˌW��
      GLfloat diff[4];  // �g�U���ˌW��
      GLfloat spec[4];  // ���ʔ��ˌW��
      GLfloat shi;      // �P���W��
    } k;

    // �ϊ�
    struct
    {
      GLfloat g[16];    // ���f���r���[�ϊ��̖@���ϊ��s��
      void loadNormalMatrix(const GgMatrix &m) { memcpy(g, m.get(), sizeof g); }
    } m;

    // �ꏊ
    struct
    {
      GLint nv;         // �@���� attribute �ϐ��̏ꏊ
      GLint lpos;       // �����ʒu�� uniform �ϐ��̏ꏊ
      GLint lamb;       // �������x�̊��������� uniform �ϐ��̏ꏊ
      GLint ldiff;      // �������x�̊g�U���ˌ������� uniform �ϐ��̏ꏊ
      GLint lspec;      // �������x�̋��ʔ��ˌ������� uniform �ϐ��̏ꏊ
      GLint kamb;       // �����̔��ˌW���� uniform �ϐ��̏ꏊ
      GLint kdiff;      // �g�U���ˌW���� uniform �ϐ��̏ꏊ
      GLint kspec;      // ���ʔ��ˌW���� uniform �ϐ��̏ꏊ
      GLint kshi;       // �P���W���� uniform �ϐ��̏ꏊ
      GLint mg;         // ���f���r���[�ϊ��̖@���ϊ��s��� uniform �ϐ��̏ꏊ
    } loc;

  public:

    // �f�X�g���N�^
    virtual ~GgSimpleShader(void) {}

    // �R���X�g���N�^
    GgSimpleShader(void) {}
    GgSimpleShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0,
      GLint nvarying = 0, const char **varyings = 0);
    GgSimpleShader(const GgSimpleShader &o)
      : GgPointShader(o), l(o.l), k(o.k), m(o.m), loc(o.loc) {}

    // ���
    GgSimpleShader &operator=(const GgSimpleShader &o)
    {
      if (&o != this)
      {
        GgPointShader::operator=(o);
        l = o.l;
        k = o.k;
        m = o.m;
        loc = o.loc;
      }
      return *this;
    }

    // �V�F�[�_�̎g�p�J�n
    virtual void use(GLuint vert, ...) const;

    // �V�F�[�_�̎g�p�I��
    virtual void unuse(void) const;

    // ����
    void setLightPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f) { l.pos[0] = x; l.pos[1] = y; l.pos[2] = z; l.pos[3] = w; }
    void setLightPosition(const GLfloat *pos) { setLightPosition(pos[0], pos[1], pos[2], pos[3]); }
    void setLightAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { l.amb[0] = r; l.amb[1] = g; l.amb[2] = b; l.amb[3] = a; }
    void setLightAmbient(const GLfloat *amb) { setLightAmbient(amb[0], amb[1], amb[2], amb[3]); }
    void setLightDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { l.diff[0] = r; l.diff[1] = g; l.diff[2] = b; l.diff[3] = a; }
    void setLightDiffuse(const GLfloat *diff) { setLightDiffuse(diff[0], diff[1], diff[2], diff[3]); }
    void setLightSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { l.spec[0] = r; l.spec[1] = g; l.spec[2] = b; l.spec[3] = a; }
    void setLightSpecular(const GLfloat *spec) { setLightSpecular(spec[0], spec[1], spec[2], spec[3]); }

    // �ގ�
    void setMaterialAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { k.amb[0] = r; k.amb[1] = g; k.amb[2] = b; k.amb[3] = a; }
    void setMaterialAmbient(const GLfloat *amb) { setMaterialAmbient(amb[0], amb[1], amb[2], amb[3]); }
    void setMaterialDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { k.diff[0] = r; k.diff[1] = g; k.diff[2] = b; k.diff[3] = a; }
    void setMaterialDiffuse(const GLfloat *diff) { setMaterialDiffuse(diff[0], diff[1], diff[2], diff[3]); }
    void setMaterialSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { k.spec[0] = r; k.spec[1] = g; k.spec[2] = b; k.spec[3] = a; }
    void setMaterialSpecular(const GLfloat *spec) { setMaterialSpecular(spec[0], spec[1], spec[2], spec[3]); }
    void setMaterialShininess(GLfloat shi) { k.shi = shi; }

    // �ϊ�
    virtual void loadMatrix(const GgMatrix &mp, const GgMatrix &mw);
    virtual void loadMatrix(const GLfloat *mp, const GLfloat *mw);
  };

}

#endif
