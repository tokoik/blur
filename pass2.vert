#version 120

// �ȑO�̃X�N���[����̒��_�ʒu
attribute vec4 p0;

// ���݂̃X�N���[����̒��_�ʒu
attribute vec4 p1;

// �ȑO�̒��_�ʒu
varying vec4 q0;

void main(void)
{
  q0 = p0;
  gl_Position = p1;
}
