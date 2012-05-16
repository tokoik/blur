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
uniform mat4 ms;    // �V���h�E�}�b�v�p�̕ϊ��s��

// ���_����
attribute vec4 pv;  // ���[�J�����W�n�̒��_�ʒu
attribute vec4 nv;  // ���_�̖@���x�N�g��

// ���ˌ����x
varying vec4 iamb;  // �����̔��ˌ����x
varying vec4 idiff; // �g�U���ˌ����x
varying vec4 ispec; // ���ʔ��ˌ����x

// �e�N�X�`�����W
varying vec2 ctex;
varying vec4 dtex;

void main(void)
{
  vec4 p = mw * pv;
  vec3 v = normalize(p.xyz / p.w);
  vec3 n = normalize(mg * nv).xyz;
  vec3 l = normalize(lpos - p).xyz;
  vec3 h = normalize(l - v);

  iamb = kamb * lamb;
  idiff = max(dot(n, l), 0.0) * kdiff * ldiff;
  ispec = pow(max(dot(n, h), 0), kshi) * kspec * lspec;
  ctex = pv.xy;
  dtex = ms * pv;
  
  gl_Position = mc * pv;
}
