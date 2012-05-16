#version 120

uniform vec4 kspec; // ‹¾–Ê”½ËŒW”

uniform sampler2D texture0;
uniform sampler2DShadow texture1;
uniform vec4 vp;
uniform vec2 rn[16];

varying vec4 iamb, idiff, ispec;
varying vec2 ctex;
varying vec4 dtex;

// s¼–Í—l‚ÌF
const vec4 c1 = vec4(0.9, 0.9, 0.9, 1.0);
const vec4 c2 = vec4(0.6, 0.7, 0.8, 1.0);

// ‰f‚è‚İ‚Ì‚Ú‚¯‹ï‡
const float sigma = 0.005;

void main(void)
{
  // s¼–Í—l
  vec4 a = mix(c1, c2, mod(floor(ctex.x * 4.0) + floor(ctex.y * 4.0), 2.0));
  vec2 p = gl_FragCoord.xy / vp.zw;
  vec4 c = texture2D(texture0, p + rn[0] * sigma);
  for (int i = 1; i < 16; ++i) c += texture2D(texture0, p + rn[i] * sigma);
  gl_FragColor = (iamb + idiff * shadow2DProj(texture1, dtex)) * a + ispec + kspec * c / 16.0;
}
