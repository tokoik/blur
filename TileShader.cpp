/*
** タイル
*/
#include <cstdarg>
#include <cmath>

#include "TileShader.h"

/*
** 一様乱数発生 (Xorshift 法)
*/
static GLfloat xor128(void)
{ 
  static unsigned int x = 123456789;
  static unsigned int y = 362436069;
  static unsigned int z = 521288629;
  static unsigned int w = 88675123; 
  unsigned int t = x ^ x << 11;
  
  x = y;
  y = z;
  z = w;
  
  return (GLfloat)(w ^= w >> 19 ^ t ^ t >> 8); 
}

/*
** 正規乱数発生 (Box Muller 法)
*/
static void boxmuller(GLfloat *r)
{
  float a = sqrt(-2.0f * log(((float)xor128() + 1.0f) / 4294967296.0f));
  float b = 6.2831853f * ((float)xor128() + 1.0f) / 4294967296.0f;
  
  r[0] = a * sin(b);
  r[1] = a * cos(b);
}

TileShader::TileShader(const char *vert, const char *frag, const char *geom) : SimpleShader(vert, frag, geom)
{
  // テクスチャユニットの uniform 変数の場所
  texture0Loc = glGetUniformLocation(program, "texture0");
  texture1Loc = glGetUniformLocation(program, "texture1");
  
  // シャドウマッピング用の uniform 変数の場所
  msLoc = glGetUniformLocation(program, "ms");
  
  // ビューポートの uniform 変数の場所
  vpLoc = glGetUniformLocation(program, "vp");
  
  // 乱数の uniform 変数の場所
  rnLoc = glGetUniformLocation(program, "rn");
  for (unsigned int i = 0; i < sizeof rn / sizeof rn[0]; i += 2) boxmuller(rn + i);
}

void TileShader::use(const GLfloat (*vert)[3], ...) const
{
  const GLfloat *norm;
  va_list list;
  va_start(list, vert);
  norm = va_arg(list, const GLfloat *);
  va_end(list);
  
  SimpleShader::use(vert, norm);

  // テクスチャユニット
  glUniform1i(texture0Loc, texture0);
  glUniform1i(texture1Loc, texture1);
  
  // 乱数
  glUniform2fv(rnLoc, sizeof rn / sizeof rn[0] / 2, rn);
  
  // シャドウマッピング用の変換行列
  glUniformMatrix4fv(msLoc, 1, GL_FALSE, ms);
  
  // ビューポート
  glUniform4f(vpLoc, (GLfloat)vp[0],  (GLfloat)vp[1], (GLfloat)vp[2], (GLfloat)vp[3]);
}
