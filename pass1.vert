#version 120

// ����
uniform vec4 lpos;  // �ʒu
uniform vec4 lamb;  // ��������
uniform vec4 ldiff; // �g�U���ˌ�����
uniform vec4 lspec; // ���ʔ��ˌ�����

// �ގ�
uniform vec4 kamb;  // �����̔��ˌW��
uniform vec4 kdiff; // �g�U���ˌW��
uniform vec4 kspec; // ���ʔ��ˌW��
uniform float kshi; // �P���W��

// �ϊ��s��
uniform mat4 mw;    // ���_���W�n�ւ̕ϊ��s��
uniform mat4 mc;    // �N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mg;    // �@���x�N�g���̕ϊ��s��

// ���_����
attribute vec4 pv;  // ���[�J�����W�n�̒��_�ʒu
attribute vec4 nv;  // ���_�̖@���x�N�g��

// ���ˌ����x
varying vec4 iamb;  // �����̔��ˌ�
varying vec4 idiff; // �g�U���ˌ�
varying vec4 ispec; // ���ʔ��ˌ�

// transform feedback
varying vec4 p1;	// ���݂̒��_�ʒu��ۑ�����t�B�[�h�o�b�N�o�b�t�@

void main(void)
{
  // �A�e�v�Z�̏���
  vec4 p = mw * pv;
  vec3 v = normalize(p.xyz / p.w);
  vec3 l = normalize((lpos * p.w - p * lpos.w).xyz);
  vec3 n = normalize((mg * nv).xyz);
  vec3 h = normalize(l - v);

  // �A�e�v�Z
  idiff = max(dot(n, l), 0.0) * kdiff * ldiff;
  ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;
  iamb = kamb * lamb;
  
  // ���_�ʒu�̎Z�o
  gl_Position = mc * pv;

  // ���݂̃X�N���[����̒��_�ʒu��ۑ�����
  p1 = gl_Position;
}
