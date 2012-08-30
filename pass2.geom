#version 120
#extension GL_EXT_geometry_shader4: enable

// �ȑO�̒��_�ʒu
varying in vec4 q0[3];

// ���X�^���C�U�ɑ��钸�_�̑��x
varying out vec2 vel;

void main(void)
{
  for (int i = 0; i < gl_VerticesIn; ++i)
  {
    vel = gl_PositionIn[i].xy / gl_PositionIn[i].w - q0[i].xy / q0[i].w;
    gl_Position = gl_PositionIn[i];
    EmitVertex();
  
    gl_Position = q0[i];
    EmitVertex();
  }
  
  vel = gl_PositionIn[0].xy / gl_PositionIn[0].w - q0[0].xy / q0[0].w;
  gl_Position = gl_PositionIn[0];
  EmitVertex();
  
  gl_Position = q0[0];
  EmitVertex();

  EndPrimitive();
}
