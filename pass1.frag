#version 120

// 反射光強度
varying vec4 iamb;	// 環境光の反射光
varying vec4 idiff;	// 拡散反射光
varying vec4 ispec;	// 鏡面反射光

// 頂点の速度ベクトルの補間値
varying vec3 vel;

void main(void)
{
  // 色バッファへの書き込み
  gl_FragData[0] = iamb + idiff + ispec;

  // 速度バッファへの書き込み
  gl_FragData[1] = vec4(vel, 1.0);
}
