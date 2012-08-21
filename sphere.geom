#version 120
#extension GL_EXT_geometry_shader4: enable

// 変換行列
uniform mat4 mc;        // クリッピング座標系への変換行列

// 反射光強度
varying out vec4 iamb;  // 環境光の反射光強度
varying out vec4 idiff; // 拡散反射光強度
varying out vec4 ispec; // 鏡面反射光強度

void main()
{
  gl_Position = mc * (gl_PositionIn[0] + vec4(-0.1, -0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(1.0, 0.0, 0.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.1, -0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(0.0, 1.0, 0.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
  gl_Position = mc * (gl_PositionIn[0] + vec4( 0.0,  0.086602540, 0.0, 0.0));
  iamb = vec4(0.0);
  idiff = vec4(0.0, 0.0, 1.0, 1.0);
  ispec = vec4(0.0);
  EmitVertex();
}
