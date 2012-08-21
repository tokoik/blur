/*
** 点を球として表示する
*/
#include "GgPointShader.h"

gg::GgPointShader::GgPointShader(const char *vert, const char *frag,
  const char *geom, GLenum input, GLenum output, int vertices,
  int nvarying, const char **varyings)
: GgShader(vert, frag, geom, input, output, vertices, nvarying, varyings)
{
  // プログラム名
  GLuint program = get();
  
  // 位置の attribute 変数の場所
  pvLoc = glGetAttribLocation(program, "pv");
  
  // 変換行列の uniform 変数の場所
  mcLoc = glGetUniformLocation(program, "mc");
}

void gg::GgPointShader::use(const GLfloat (*vert)[3], ...) const
{
  // 基底クラスのシェーダの設定を呼び出す
  GgShader::use(0);
  
  // attribute 変数 pv を配列変数から得ることを許可する
  glEnableVertexAttribArray(pvLoc);
  
  // attribute 変数 pv と配列変数 vert を結びつける
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);

  // 変換行列を設定する
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
}
