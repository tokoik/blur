#version 120

// �ȑO�̃X�N���[����̒��_�ʒu
attribute vec3 p0;

// ���݂̃X�N���[����̒��_�ʒu
attribute vec3 p1;

// �ȑO�̒��_�ʒu
varying vec3 q0;

void main(void)
{
  q0 = p0;
  gl_Position = vec4(p1, 1.0);
}
