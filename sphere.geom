#version 120
#extension GL_EXT_geometry_shader4: enable

// 光源
uniform vec4 lpos;      // 位置
uniform vec4 lamb;      // 環境光成分
uniform vec4 ldiff;     // 拡散反射光成分
uniform vec4 lspec;     // 鏡面反射光成分

// 材質
uniform vec4 kamb;      // 環境光の反射係数
uniform vec4 kdiff;     // 拡散反射係数
uniform vec4 kspec;     // 鏡面反射係数
uniform float kshi;     // 輝き係数

// 変換行列
uniform mat4 mw;        // 視点座標系への変換行列
uniform mat4 mc;        // クリッピング座標系への変換行列
uniform mat4 mg;        // 法線ベクトルの変換行列

// 反射光強度
varying out vec4 iamb;  // 環境光の反射光強度
varying out vec4 idiff; // 拡散反射光強度
varying out vec4 ispec; // 鏡面反射光強度

void main()
{
  iamb = kamb;
  idiff = kdiff;
  ispec = kspec;
  gl_Position = mc * (gl_PositionIn[0] + vec4(-0.1, -0.086602540, 0.0, 0.0));
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.1, -0.086602540, 0.0, 0.0));
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.0,  0.086602540, 0.0, 0.0));
  EmitVertex();
}
