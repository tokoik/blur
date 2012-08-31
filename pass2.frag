#version 120

// 露光時間比
const float exp_rate = 0.8;

// 露光遅延
const float exp_delay = 0.3;

// 乱数
uniform vec2 rn[16];

// 色バッファ
uniform sampler2D texture0;

// 速度バッファ
uniform sampler2D texture1;

// スクリーン上の位置
varying vec2 t;

// v 方向の n 画素の色の平均を求める
vec4 average(vec2 v, int n)
{
  vec4 c = vec4(0.0);

  for (int i = 0; i < n; ++i)
  {
    c += texture2D(texture0, t + v * (float(i) / float(n) * exp_rate - exp_delay));
  }

  return c / float(n);
}

void main(void)
{
  vec4 v = texture2D(texture1, t);

  if (v.a != 0.0)
  {
    // フラグメントがオブジェクト上ならそこをぼかす
    gl_FragColor = average(v.xy, 16);
  }
  else
  {
    // フラグメントが背景なら
    gl_FragColor = texture2D(texture1, t);
  }
}
