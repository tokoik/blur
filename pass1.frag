#version 120

// 反射光強度
varying vec4 iamb;	// 環境光の反射光
varying vec4 idiff;	// 拡散反射光
varying vec4 ispec;	// 鏡面反射光

void main(void)
{
  // 色バッファへの書き込み
  gl_FragColor = iamb + idiff + ispec;
}
