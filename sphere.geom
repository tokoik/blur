#version 120
#extension GL_EXT_geometry_shader4: enable

// ����
uniform vec4 lpos;      // �ʒu
uniform vec4 lamb;      // ��������
uniform vec4 ldiff;     // �g�U���ˌ�����
uniform vec4 lspec;     // ���ʔ��ˌ�����

// �ގ�
uniform vec4 kamb;      // �����̔��ˌW��
uniform vec4 kdiff;     // �g�U���ˌW��
uniform vec4 kspec;     // ���ʔ��ˌW��
uniform float kshi;     // �P���W��

// �ϊ��s��
uniform mat4 mw;        // ���_���W�n�ւ̕ϊ��s��
uniform mat4 mc;        // �N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mg;        // �@���x�N�g���̕ϊ��s��

// ���ˌ����x
varying out vec4 iamb;  // �����̔��ˌ����x
varying out vec4 idiff; // �g�U���ˌ����x
varying out vec4 ispec; // ���ʔ��ˌ����x

void main()
{
  iamb = kamb;
  idiff = kdiff;
  ispec = kspec;
  gl_Position = mc * (gl_PositionIn[0] + vec4(-0.1, -0.086602540, 0.0, 0.0));
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.1, -0.086602540, 0.0, 0.0));
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.0,  0.086602540, 0.0, 0.0));
  EmitVertex();
}
