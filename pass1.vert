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
varying vec4 iamb;  // �����̔��ˌ����x
varying vec4 idiff; // �g�U���ˌ����x
varying vec4 ispec; // ���ʔ��ˌ����x

// transform feedback
attribute vec3 p0;
varying vec3 p1;
varying vec2 vel;

void main(void)
{
  vec4 p = mw * pv;
  vec3 v = normalize(p.xyz / p.w);
  vec3 l = normalize((lpos * p.w - p * lpos.w).xyz);
  vec3 n = normalize((mg * nv).xyz);
  vec3 h = normalize(l - v);

  idiff = max(dot(n, l), 0.0) * kdiff * ldiff;
  ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;
  iamb = kamb * lamb;
  
  gl_Position = mc * pv;
  p1 = gl_Position.xyz / gl_Position.w;
  vel = (p1 - p0).xy;
  iamb = vec4(normalize(vel) * 0.5 + 0.5, 0.0, 1.0);
  idiff = ispec = vec4(0.0);
}
