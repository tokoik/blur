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

// 頂点属性
attribute vec4 pv;  // ローカル座標系の頂点位置
attribute vec4 nv;  // 頂点の法線ベクトル

// 反射光強度
varying vec4 iamb;  // 環境光の反射光
varying vec4 idiff; // 拡散反射光
varying vec4 ispec; // 鏡面反射光

// transform feedback
attribute vec3 p0;	// スクリーン上での以前の頂点位置
varying vec3 p1;	// 現在の頂点位置を保存するフィードバックバッファ
varying vec2 vel;	// 現在の頂点の速度ベクトル

void main(void)
{
  // 陰影計算の準備
  vec4 p = mw * pv;
  vec3 v = normalize(p.xyz / p.w);
  vec3 l = normalize((lpos * p.w - p * lpos.w).xyz);
  vec3 n = normalize((mg * nv).xyz);
  vec3 h = normalize(l - v);

  // 陰影計算
  idiff = max(dot(n, l), 0.0) * kdiff * ldiff;
  ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;
  iamb = kamb * lamb;
  
  // 頂点位置の算出
  gl_Position = mc * pv;

  // 現在の頂点位置を保存する
  p1 = gl_Position.xyz / gl_Position.w;

  // 頂点の速度ベクトルをラスタライザに送る
  vel = (p1 - p0).xy;
}
