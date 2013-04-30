#version 120

// 露光時間比
const float exp_rate = 0.8;

// 露光遅延
const float exp_delay = 0.3;

// サンプル数
const int samples = 16;

// 乱数
uniform vec2 rn[16];

// 色バッファ
uniform sampler2D texture0;

// 速度バッファ
uniform sampler2D texture1;

// スクリーン上の位置
varying vec2 t;

// v 方向の n 画素の色の平均を求める
vec4 average(in vec2 v, in int n)
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
    gl_FragColor = average(v.xy, samples);
  }
  else
  {
    // フラグメントが背景なら
#if 1
    int count = 0;
    vec4 d = vec4(0.0);
    
    for (int i = 0; i < 16; ++i)
    {
      // そのフラグメントの周囲をランダムにサンプリングして
      vec4 p = texture2D(texture1, t + rn[i]);
      
      if (p.a != 0.0)
      {
        // オブジェクト上のフラグメントが見つかったら
#if 1
        vec4 q = texture2D(texture1, t + p.xy);
        
        if (q.a != 0.0)
        {
          // オブジェクト上のフラグメントを見つけたらそこをぼかす
          d += average(q.xy, samples);
          ++count;
        }
#else
        d += average(p.xy, samples);
        ++count;
#endif
      }
    }
    
    if (count == 0)
      gl_FragColor = texture2D(texture0, t);
    else
      gl_FragColor = d / float(count);
#else
    gl_FragColor = texture2D(texture0, t);
#endif
  }
}
