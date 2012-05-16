#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H

/*
** �P���ȉA�e�t��
*/
#include "Shader.h"
#include "Matrix.h"

class SimpleShader : public Shader
{
  // ����
  GLfloat lpos[4];    // �����ʒu
  GLint lposLoc;      // �����ʒu�� uniform �ϐ��̏ꏊ
  GLfloat lamb[4];    // �������x�̊�������
  GLint lambLoc;      // �������x�̊��������� uniform �ϐ��̏ꏊ
  GLfloat ldiff[4];   // �������x�̊g�U���ˌ�����
  GLint ldiffLoc;     // �������x�̊g�U���ˌ������� uniform �ϐ��̏ꏊ
  GLfloat lspec[4];   // �������x�̋��ʔ��ˌ�����
  GLint lspecLoc;     // �������x�̋��ʔ��ˌ������� uniform �ϐ��̏ꏊ
  GLfloat kamb[4];    // �����̔��ˌW��
  GLint kambLoc;      // �����̔��ˌW���� uniform �ϐ��̏ꏊ
  
  // �ގ�
  GLfloat kdiff[4];   // �g�U���ˌW��
  GLint kdiffLoc;     // �g�U���ˌW���� uniform �ϐ��̏ꏊ
  GLfloat kspec[4];   // ���ʔ��ˌW��
  GLint kspecLoc;     // ���ʔ��ˌW���� uniform �ϐ��̏ꏊ
  GLfloat kshi;       // �P���W��
  GLint kshiLoc;      // �P���W���� uniform �ϐ��̏ꏊ

  // �ϊ�
  GLfloat mc[16];     // ���f���r���[�E���e�ϊ��s��
  GLint mcLoc;        // ���f���r���[�E���e�ϊ��s��� uniform �ϐ��̏ꏊ
  GLfloat mw[16];     // ���f���r���[�ϊ��s��
  GLint mwLoc;        // ���f���r���[�ϊ��s��� uniform �ϐ��̏ꏊ
  GLfloat mg[16];     // ���f���r���[�ϊ��̖@���ϊ��s��
  GLint mgLoc;        // ���f���r���[�ϊ��̖@���ϊ��s��� uniform �ϐ��̏ꏊ
  
  // ����
  GLint pvLoc;        // ���_�ʒu�� attribute �ϐ��̏ꏊ
  GLint nvLoc;        // ���_�@���� attribute �ϐ��̏ꏊ
  
public:
  SimpleShader(void) {};
  SimpleShader(const char *vert, const char *frag, const char *geom = 0);
  virtual ~SimpleShader(void) {}
  virtual void use(const GLfloat (*vert)[3], ...) const;

  // ����
  void setLightPosition(const GLfloat *pos) { setLightPosition(pos[0], pos[1], pos[2], pos[3]); }
  void setLightPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f) { lpos[0] = x; lpos[1] = y; lpos[2] = z; lpos[3] = w; }
  void setLightAmbient(const GLfloat *amb) { setLightAmbient(amb[0], amb[1], amb[2], amb[3]); }
  void setLightAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { lamb[0] = r; lamb[1] = g; lamb[2] = b; lamb[3] = a; }
  void setLightDiffuse(const GLfloat *diff) { setLightDiffuse(diff[0], diff[1], diff[2], diff[3]); }
  void setLightDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { ldiff[0] = r; ldiff[1] = g; ldiff[2] = b; ldiff[3] = a; }
  void setLightSpecular(const GLfloat *spec) { setLightSpecular(spec[0], spec[1], spec[2], spec[3]); }
  void setLightSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { lspec[0] = r; lspec[1] = g; lspec[2] = b; lspec[3] = a; }
  
  // �ގ�
  void setMaterialAmbient(const GLfloat *amb) { setMaterialAmbient(amb[0], amb[1], amb[2], amb[3]); }
  void setMaterialAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kamb[0] = r; kamb[1] = g; kamb[2] = b; kamb[3] = a; }
  void setMaterialDiffuse(const GLfloat *diff) { setMaterialDiffuse(diff[0], diff[1], diff[2], diff[3]); }
  void setMaterialDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kdiff[0] = r; kdiff[1] = g; kdiff[2] = b; kdiff[3] = a; }
  void setMaterialSpecular(const GLfloat *spec) { setMaterialSpecular(spec[0], spec[1], spec[2], spec[3]); }
  void setMaterialSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) { kspec[0] = r; kspec[1] = g; kspec[2] = b; kspec[3] = a; }
  void setMaterialShininess(GLfloat shi) { kshi = shi; }

  // �ϊ�
  void loadModelViewPerspectiveMatrix(const GLfloat *m) { memcpy(mc, m, sizeof mc); }
  void loadModelViewPerspectiveMatrix(const Matrix &m) { memcpy(mc, m.get(), sizeof mc); }
  void loadModelViewMatrix(const GLfloat *m) { memcpy(mw, m, sizeof mw); }
  void loadModelViewMatrix(const Matrix &m) { memcpy(mw, m.get(), sizeof mw); }
  void loadNormalMatrix(const GLfloat *m) { memcpy(mg, m, sizeof mg); }
  void loadNormalMatrix(const Matrix &m) { memcpy(mg, m.get(), sizeof mg); }
  void loadMatrix(const Matrix &mp, const Matrix &mw);
};

#endif
