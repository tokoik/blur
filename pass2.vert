#version 120

// 以前のスクリーン上の頂点位置
attribute vec3 p0;

// 現在のスクリーン上の頂点位置
attribute vec3 p1;

// 以前の頂点位置
varying vec3 q0;

void main(void)
{
  q0 = p0;
  gl_Position = vec4(p1, 1.0);
}
