#version 120
#extension GL_EXT_geometry_shader4: enable

// �ȑO�̒��_�ʒu
varying in vec3 q0[];

// ���X�^���C�U�ɑ��钸�_�̑��x
varying out vec2 vel;

// Triangle Strip ����o�͂���
void strip(in int i, in int j, in vec2 v[3])
{
  vel = vec2(1.0, 0.0);
  gl_Position = gl_PositionIn[i];
  EmitVertex();
  return;

  vel = v[i];
  gl_Position = gl_PositionIn[i];
  EmitVertex();
  gl_Position = vec4(q0[i], 1.0);
  EmitVertex();
  vel = v[j];
  gl_Position = gl_PositionIn[j];
  EmitVertex();
  gl_Position = vec4(q0[j], 1.0);
  EmitVertex();
  EndPrimitive();
}

void main(void)
{
  // �O���_�̑��x�����߂�
  vec2 v[3];
  v[0] = gl_PositionIn[0].xy - q0[0].xy;
  v[1] = gl_PositionIn[1].xy - q0[1].xy;
  v[2] = gl_PositionIn[2].xy - q0[2].xy;

  // Triangle Strip ���O�o�͂���
  strip(0, 1, v);
  strip(1, 2, v);
  strip(2, 0, v);
}
