#version 120

// 光源
uniform vec4 lpos;  // 位置
uniform vec4 lamb;  // 環境光成分
uniform vec4 ldiff; // 拡散反射光成分
uniform vec4 lspec; // 鏡面反射光成分

// 材質
uniform vec4 kamb;  // 環境光の反射係数
uniform vec4 kdiff; // 拡散反射係数
uniform vec4 kspec; // 鏡面反射係数
uniform float kshi; // 輝き係数

// 変換行列
uniform mat4 mw;    // 視点座標系への変換行列
uniform mat4 mc;    // クリッピング座標系への変換行列
uniform mat4 mg;    // 法線ベクトルの変換行列
uniform mat4 ms;    // シャドウマップ用の変換行列

// 頂点属性
attribute vec4 pv;  // ローカル座標系の頂点位置
attribute vec4 nv;  // 頂点の法線ベクトル

// 反射光強度
varying vec4 iamb;  // 環境光の反射光強度
varying vec4 idiff; // 拡散反射光強度
varying vec4 ispec; // 鏡面反射光強度

// テクスチャ座標
varying vec2 ctex;
varying vec4 dtex;

void main(void)
{
  vec4 p = mw * pv;
  vec3 v = normalize(p.xyz / p.w);
  vec3 n = normalize(mg * nv).xyz;
  vec3 l = normalize(lpos - p).xyz;
  vec3 h = normalize(l - v);

  iamb = kamb * lamb;
  idiff = max(dot(n, l), 0.0) * kdiff * ldiff;
  ispec = pow(max(dot(n, h), 0), kshi) * kspec * lspec;
  ctex = pv.xy;
  dtex = ms * pv;
  
  gl_Position = mc * pv;
}
