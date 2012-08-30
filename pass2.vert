#version 120

// 以前のスクリーン上の頂点位置
attribute vec4 p0;

// 現在のスクリーン上の頂点位置
attribute vec4 p1;

// 以前の頂点位置
varying vec4 q0;

void main(void)
{
  q0 = p0;
  gl_Position = p1;
}
