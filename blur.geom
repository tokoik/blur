#version 120
#extension GL_EXT_geometry_shader4: enable

// 反射光強度
varying in vec4 iamb[];  // 環境光の反射光強度
varying in vec4 idiff[]; // 拡散反射光強度
varying in vec4 ispec[]; // 鏡面反射光強度

// 以前の頂点位置
varying in vec3 pos[];
varying in vec2 vel[];

// カラーバッファに書き込む値
varying out vec4 dc;
varying out vec4 sc;

// 速度バッファに書き込む値
varying out vec2 v;
varying out vec3 p1;

void main()
{
  gl_Position = gl_PositionIn[0];
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[0] + idiff[0];
  sc = ispec[0];
  v = vel[0];
  EmitVertex();
  gl_Position = vec4(pos[0], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[0] + idiff[0];
  sc = ispec[0];
  v = vel[0];
  EmitVertex();
  gl_Position = gl_PositionIn[1];
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[1] + idiff[1];
  sc = ispec[1];
  v = vel[1];
  EmitVertex();
  gl_Position = vec4(pos[1], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[1] + idiff[1];
  sc = ispec[1];
  v = vel[1];
  EmitVertex();
  EndPrimitive();

  gl_Position = gl_PositionIn[1];
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[1] + idiff[1];
  sc = ispec[1];
  v = vel[1];
  EmitVertex();
  gl_Position = vec4(pos[1], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[1] + idiff[1];
  sc = ispec[1];
  v = vel[1];
  EmitVertex();
  gl_Position = gl_PositionIn[2];
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[2] + idiff[2];
  sc = ispec[2];
  v = vel[2];
  EmitVertex();
  gl_Position = vec4(pos[2], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[2] + idiff[2];
  sc = ispec[2];
  v = vel[2];
  EmitVertex();
  EndPrimitive();

  gl_Position = gl_PositionIn[2];
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[2] + idiff[2];
  sc = ispec[2];
  v = vel[2];
  EmitVertex();
  gl_Position = vec4(pos[2], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[2] + idiff[2];
  sc = ispec[2];
  v = vel[2];
  EmitVertex();
  p1 = gl_Position.xyz / gl_Position.w;
  gl_Position = gl_PositionIn[0];
  dc = iamb[0] + idiff[0];
  sc = ispec[0];
  v = vel[0];
  EmitVertex();
  gl_Position = vec4(pos[0], 1.0);
  p1 = gl_Position.xyz / gl_Position.w;
  dc = iamb[0] + idiff[0];
  sc = ispec[0];
  v = vel[0];
  EmitVertex();
  EndPrimitive();
}
