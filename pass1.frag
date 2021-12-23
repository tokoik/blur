#version 120

// 光源
uniform vec4 ldiff;		// 拡散反射光成分
uniform vec4 lspec;		// 鏡面反射光成分

// 材質
uniform vec4 kdiff;		// 拡散反射係数
uniform vec4 kspec;		// 鏡面反射係数

// 閾値
uniform float tdiff = 0.5;	// 拡散反射光強度の閾値
uniform float tspec = 0.3;	// 鏡面反射光強度の閾値

// 反射光強度
varying vec4 iamb;		// 環境光の反射光強度
varying float idiff;	// 拡散反射光の反射率
varying float ispec;	// 鏡面反射光の反射率

void main(void)
{
  // 色バッファへの書き込み
  gl_FragData[0] = iamb + step(tdiff, idiff) * kdiff * ldiff + step(tspec, ispec) * kspec * lspec;
}
