#version 120

// クリッピング空間を覆うポリゴンの頂点位置
attribute vec4 pv;

// 頂点の位置から求めるテクスチャ座標
varying vec2 t;

void main(void)
{
  // 頂点位置はそのままラスタライザに送る
  gl_Position = pv;

  // ラスタライザで頂点位置を補間してクリッピング空間中の画素位置を求める
  t = pv.xy * 0.5 + 0.5;
}
