#version 120
#extension GL_EXT_geometry_shader4: enable

// �ϊ��s��
uniform mat4 mc;        // �N���b�s���O���W�n�ւ̕ϊ��s��

// ���ˌ����x
varying out vec4 iamb;  // �����̔��ˌ����x
varying out vec4 idiff; // �g�U���ˌ����x
varying out vec4 ispec; // ���ʔ��ˌ����x

void main()
{
  gl_Position = mc * (gl_PositionIn[0] + vec4(-0.1, -0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(1.0, 0.0, 0.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.1, -0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(0.0, 1.0, 0.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.0,  0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(0.0, 0.0, 1.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
}
