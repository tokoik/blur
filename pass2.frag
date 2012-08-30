#version 120

// フラグメントの速度
varying vec2 vel;

void main(void)
{
  gl_FragColor = vec4(vel, 0.0, 1.0);
}
