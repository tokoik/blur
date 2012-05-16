#ifndef TILESHADER_H
#define TILESHADER_H

/*
** �^�C��
*/
#include "SimpleShader.h"
#include "Matrix.h"

#define SAMPLES 16

class TileShader : public SimpleShader
{
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
  GLfloat rn[SAMPLES * 2];  // ����
  GLint rnLoc;              // ������ uniform �ϐ��̏ꏊ
  
  // �e�N�X�`�����j�b�g�̑I��
  void active(GLuint u, GLuint t) const { glActiveTexture(GL_TEXTURE0 + u); glBindTexture(GL_TEXTURE_2D, t); }
  
public:
  TileShader(void) {};
  TileShader(const char *vert, const char *frag, const char *geom = 0);
  virtual ~TileShader(void) {}
  virtual void use(const GLfloat (*vert)[3], ...) const;

  // �e�N�X�`�����j�b�g
  void setTexture0(GLuint unit, GLuint tex) { active(texture0 = unit, tex); }
  void setTexture1(GLuint unit, GLuint tex) { active(texture1 = unit, tex); }
  
  // �V���h�E�}�b�s���O�p�̕ϊ��s��
  void loadShadowMatrix(const GLfloat *m) { memcpy(ms, m, sizeof ms); }
  void loadShadowMatrix(const Matrix &m) { loadShadowMatrix(m.get()); }
  
  // �r���[�|�[�g
  void saveViewport(void) { glGetIntegerv(GL_VIEWPORT, vp); }
  void restoreViewport(void) { glViewport(vp[0], vp[1], vp[2], vp[3]); }
};

#endif
