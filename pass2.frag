#version 120

// �t���O�����g�̑��x
varying vec2 vel;

void main(void)
{
  gl_FragColor = vec4(vel, 0.0, 1.0);
}
