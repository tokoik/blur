#version 120

// 頂点属性
attribute vec4 pv;  // ローカル座標系の頂点位置

void main(void)
{
  gl_Position = pv;
}
