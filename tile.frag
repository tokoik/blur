#version 120

uniform vec4 kspec;   // 鏡面反射係数

uniform sampler2D texture0;
uniform sampler2DShadow texture1;

uniform vec4 vp;      // ビューポート
uniform vec2 rn[16];  // 二次元正規乱数

varying vec4 iamb, idiff, ispec;
varying vec2 ctex;
varying vec4 dtex;

// 市松模様の色
const vec4 c1 = vec4(0.9, 0.9, 0.9, 1.0);
const vec4 c2 = vec4(0.6, 0.7, 0.8, 1.0);

void main(void)
{
  // 市松模様
  vec4 a = mix(c1, c2, mod(floor(ctex.x * 4.0) + floor(ctex.y * 4.0), 2.0));
  vec2 p = gl_FragCoord.xy / vp.zw;
  vec4 c = texture2D(texture0, p);
  gl_FragColor = (iamb + idiff) * a + ispec + kspec * c;
}
