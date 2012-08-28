#version 120

attribute vec4 pv;
varying vec2 t;

void main(void)
{
  gl_Position = pv;
  t = pv.xy * 0.5 + 0.5;
}
