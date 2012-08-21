/*
** ゲームグラフィックス特論用補助プログラム
**

Copyright (c) 2011, 2012 Kohe Tokoi. All Rights Reserved.

Permission is hereby granted, free of charge,  to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction,  including without limitation the rights 
to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell 
copies or substantial portions of the Software.

The above  copyright notice  and this permission notice  shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR 
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY, 
FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL 
KOHE TOKOI  BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY,  WHETHER IN 
AN ACTION  OF CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM,  OUT OF  OR  IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <float.h>

#include "gg.h"

/*
** ゲームグラフィックス特論の都合にもとづく初期化
*/
void gg::ggInit(void)
{
  // バージョンチェック
  if (atof(reinterpret_cast<const char *>(glGetString(GL_VERSION))) < 2.1)
  {
    std::cerr << "Error: This program requires OpenGL 2.1 or lator." << std::endl;
    exit(1);
  }

  // Swap Interval の設定
#if defined(X11)
  Display *dpy = glXGetCurrentDisplay();
  GLXDrawable drawable = glXGetCurrentDrawable();
  if (drawable)
  {
    glXSwapIntervalEXT(dpy, drawable, 1);
    return;
  }
#elif defined(__APPLE__)
  int swap_interval = 1;
  CGLContextObj cgl_context = CGLGetCurrentContext();
  if (!CGLSetParameter(cgl_context, kCGLCPSwapInterval, &swap_interval))
    return;
#elif defined(WIN32)
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }
#  if defined(WGLEW_EXT_swap_control)
  if (WGLEW_EXT_swap_control)
  {
    wglSwapIntervalEXT(1);
    return;
  }
#  endif
#else
  std::cerr << "Warning: Could not set swap interval" << std::endl;
#endif
}

/*
** OpenGL のエラーチェック
*/
void gg::ggError(const char *msg)
{
  GLenum error = glGetError();

  if (error != GL_NO_ERROR)
  {
    if (msg) std::cerr << msg << ": ";

    switch (error)
    {
    case GL_INVALID_ENUM:
      std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
      break;
    case GL_INVALID_VALUE:
      std::cerr << "A numeric argument is out of range" << std::endl;
      break;
    case GL_INVALID_OPERATION:
      std::cerr << "The specified operation is not allowed in the current state" << std::endl;
      break;
    case GL_STACK_OVERFLOW:
      std::cerr << "This command would cause a stack overflow" << std::endl;
      break;
    case GL_STACK_UNDERFLOW:
      std::cerr << "This command would cause a a stack underflow" << std::endl;
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr << "There is not enough memory left to execute the command" << std::endl;
      break;
    case GL_TABLE_TOO_LARGE:
      std::cerr << "The specified table exceeds the implementation's maximum supported table size" << std::endl;
      break;
#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#    define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#endif
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;
      break;
    default:
      std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;
      break;
    }
  }
}

/*
** FBO のエラーチェック
*/
void gg::ggFBOError(const char *msg)
{
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

  if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    if (msg) std::cerr << msg << ": ";

    switch (status)
    {
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      std::cerr << "Unsupported framebuffer format" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      std::cerr << "Framebuffer incomplete, missing attachment" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      std::cerr << "Framebuffer incomplete, duplicate attachment" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      std::cerr << "Framebuffer incomplete, attached images must have same dimensions" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      std::cerr << "Framebuffer incomplete, attached images must have same format" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      std::cerr << "Framebuffer incomplete, missing draw buffer" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      std::cerr << "Framebuffer incomplete, missing read buffer" << std::endl;
      break;
    default:
      std::cerr << "Programming error; will fail on all hardware: " << std::hex << std::showbase << status << std::endl;
      break;
    }
  }
}

/*
** シェーダーのソースプログラムをメモリに読み込む
*/
static bool readShaderSource(GLuint shader, const char *name)
{
  bool ret = true; // 終了ステータス（true ならエラー）
  
  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  if (file.fail())
  {
    // 開けなかった
    std::cerr << "Error: Can't open source file: " << name << std::endl;
  }
  else
  {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    
    // ファイルサイズのメモリを確保
    char *buffer = new GLchar[length];
    
    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(buffer, length);

    if (file.bad())
    {
      // うまく読み込めなかった
      std::cerr << "Error: Could not read souce file: " << name << std::endl;
    }
    else {
      // シェーダのソースプログラムのシェーダオブジェクトへの読み込み
      const GLchar *source[] = { buffer };
      glShaderSource(shader, 1, source, &length);
      ret = false;
    }
    file.close();
    
    // 読み込みに使ったメモリを開放する
    delete[] buffer;
  }
  
  return ret;
}

/*
** シェーダの情報を表示する
*/
static void printShaderInfoLog(GLuint shader)
{
  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // シェーダのコンパイル時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** プログラムの情報を表示する
*/
static void printProgramInfoLog(GLuint program)
{
  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // シェーダのリンク時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** シェーダーソースファイルの読み込み
*/
GLuint gg::loadShader(
  const char *vert,       // バーテックスシェーダのソースファイル名
  const char *frag,       // フラグメントシェーダのソースファイル名
  const char *geom,       // ジオメトリシェーダのソースファイル名
  GLenum input,           // ジオメトリシェーダの入力プリミティブ
  GLenum output,          // ジオメトリシェーダの出力プリミティブ
  int vertices,           // ジオメトリシェーダの出力頂点数
  int nvarying,           // Transform Feedback する varying 変数の数
  const char **varyings   // Transform Feedback する varying 変数のリスト
  )
{
  // シェーダプログラムの作成
  GLuint program = glCreateProgram();

  if (program > 0)
  {
    GLint compiled, linked; // コンパイル結果, リンク結果

    // バーテックスシェーダの作成
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    
    // バーテックスシェーダのソースプログラムの読み込み
    if (readShaderSource(vertShader, vert))
    {
      glDeleteShader(vertShader);
      glDeleteProgram(program);
      return 0;
    }
    
    // バーテックスシェーダのコンパイル
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    printShaderInfoLog(vertShader);
    if (compiled == GL_FALSE)
    {
      std::cerr << "Error: Could not compile vertex shader source: " << vert << std::endl;
      glDeleteShader(vertShader);
      glDeleteProgram(program);
      return 0;
    }
    
    // バーテックスシェーダシェーダプログラムへの組み込み
    glAttachShader(program, vertShader);
    glDeleteShader(vertShader);

    if (frag)
    {
      // フラグメントシェーダの作成
      GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
      
      // フラグメントシェーダのソースプログラムの読み込み
      if (readShaderSource(fragShader, frag))
      {
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // フラグメントシェーダのコンパイル
      glCompileShader(fragShader);
      glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
      printShaderInfoLog(fragShader);
      if (compiled == GL_FALSE)
      {
        std::cerr << "Error: Could not compile fragment shader source: " << frag << std::endl;
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // フラグメントシェーダシェーダプログラムへの組み込み
      glAttachShader(program, fragShader);
      glDeleteShader(fragShader);
    }

    if (geom)
    {
      // ジオメトリシェーダの作成
      GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
      
      // ジオメトリシェーダのソースプログラムの読み込み
      if (readShaderSource(geomShader, geom))
      {
        glDeleteShader(geomShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // ジオメトリシェーダのコンパイル
      glCompileShader(geomShader);
      glGetShaderiv(geomShader, GL_COMPILE_STATUS, &compiled);
      printShaderInfoLog(geomShader);
      if (compiled == GL_FALSE)
      {
        std::cerr << "Error: Could not compile geometry shader source: " << geom << std::endl;
        glDeleteShader(geomShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // ジオメトリシェーダのシェーダプログラムへの組み込み
      glAttachShader(program, geomShader);
      glDeleteShader(geomShader);

      // ジオメトリシェーダに入力する基本図形の指定
      glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, input);
      
      // ジオメトリシェーダから出力する基本図形の指定
      glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);

      // ジオメトリシェーダが出力する頂点数の設定
      if (vertices > 0) glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
    }

    // feedback に使う varying 変数を指定する
    if (nvarying > 0) glTransformFeedbackVaryings(program, nvarying, varyings, GL_SEPARATE_ATTRIBS);

    // シェーダプログラムのリンク
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    printProgramInfoLog(program);
    if (linked == GL_FALSE)
    {
      std::cerr << "Error: Could not link shader program" << std::endl;
      glDeleteProgram(program);
      return 0;
    }
  }

  return program;
}

/*
** テクスチャマッピング用の RAW 画像ファイルの読み込み
*/
void gg::loadImage(const char *name, int width, int height, GLenum format)
{
  // テクスチャの読み込み先
  char *image = 0;
  
  // テクスチャファイルを開く
  std::ifstream file(name, std::ios::binary);

  if (file.fail())
  {
    // 開けなかった
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else
  {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());

    // テクスチャサイズ分のメモリを確保する
    GLsizei maxsize = width * height * ((format == GL_RGB) ? 3 : 4);
    image = new char[maxsize];

    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(image, (size < maxsize) ? size : maxsize);

    if (file.bad())
    {
      // うまく読み込めなかった
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
 }
  
  // format が RGBA なら 4 バイト境界に設定
  glPixelStorei(GL_UNPACK_ALIGNMENT, (format == GL_RGBA) ? 4 : 1);
  
  // テクスチャを割り当てる
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
  
  // バイリニア（ミップマップなし），エッジでクランプ
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  // 読み込みに使ったメモリを開放する
  delete[] image;
}

/*
** 高さマップ用の RAW 画像ファイルの読み込んで法線マップを作成する
*/
void gg::loadHeight(const char *name, int width, int height, float nz)
{
  // 作成する法線マップ
  GLfloat (*nmap)[4] = 0;
  
  // テクスチャファイルを開く
  std::ifstream file(name, std::ios::binary);
  
  if (file.fail())
  {
    // 開けなかった
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else
  {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());
    
    // メモリを確保する
    GLsizei maxsize = width * height;
    GLubyte *hmap = 0;
    try {
      hmap = new GLubyte[maxsize];
      nmap = new GLfloat[maxsize][4];
    }
    catch (std::bad_alloc e) {
      delete[] hmap;
      delete[] nmap;
      throw e;
    }
    
    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(reinterpret_cast<char *>(hmap), (size < maxsize) ? size : maxsize);
    
    if (file.bad())
    {
      // うまく読み込めなかった
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
    
    // 法線マップの作成
    for (int i = 0; i < maxsize; ++i)
    {
      int x = i % width, y = i - x;
      
      // 隣接する画素との値の差を法線ベクトルの成分に用いる
      float nx = static_cast<float>(hmap[y + (x + 1) % width] - hmap[i]);
      float ny = static_cast<float>(hmap[(y + width) % maxsize + x] - hmap[i]);
      
      // 法線ベクトルの長さを求めておく
      float nl = sqrt(nx * nx + ny * ny + nz * nz);
      
      // 法線ベクトルを求める
      nmap[i][0] = nx * 0.5f / nl + 0.5f;
      nmap[i][1] = ny * 0.5f / nl + 0.5f;
      nmap[i][2] = nz * 0.5f / nl + 0.5f;
      nmap[i][3] = hmap[i] * 0.0039215686f; // = 1/255
    }

    // 高さマップの読み込みに使ったメモリを開放する
    delete[] hmap;
  }
  
  // nmap が GLfloat なので 4 バイト境界に設定
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  // テクスチャを割り当てる
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nmap);
  
  // バイリニア（ミップマップなし），エッジでクランプ
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  
  // 法線マップの作成に使ったメモリを解放する
  delete[] nmap;
}

/*
** 変換行列：行列とベクトルの積 c ← a × b
*/
void gg::GgMatrix::projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 4; ++i)
  {
    c[i] = a[0 + i] * b[0] + a[4 + i] * b[1] + a[8 + i] * b[2] + a[12 + i] * b[3];
  }
}

/*
** 変換行列：行列と行列の積 c ← a × b
*/
void gg::GgMatrix::multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 16; ++i)
  {
    int j = i & 3, k = i & ~3;
    
    c[i] = a[0 + j] * b[k + 0] + a[4 + j] * b[k + 1] + a[8 + j] * b[k + 2] + a[12 + j] * b[k + 3];
  }
}

/*
** 変換行列：単位行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadIdentity(void)
{
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[ 0] = array[ 5] = array[10] = array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：平行移動変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  array[12] = x;
  array[13] = y;
  array[14] = z;
  array[ 0] = array[ 5] = array[10] = array[15] = w;
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = 0.0f;
  
  return *this;
}

/*
** 変換行列：拡大縮小変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  array[ 0] = x;
  array[ 5] = y;
  array[10] = z;
  array[15] = w;
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  
  return *this;
}

/*
** 変換行列：x 軸中心の回転変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadRotateX(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = 1.0f; array[ 1] = 0.0f; array[ 2] = 0.0f; array[ 3] = 0.0f;
  array[ 4] = 0.0f; array[ 5] = c;    array[ 6] = s;    array[ 7] = 0.0f;
  array[ 8] = 0.0f; array[ 9] = -s;   array[10] = c;    array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：y 軸中心の回転変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadRotateY(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = c;    array[ 1] = 0.0f; array[ 2] = -s;   array[ 3] = 0.0f;
  array[ 4] = 0.0f; array[ 5] = 1.0f; array[ 6] = 0.0f; array[ 7] = 0.0f;
  array[ 8] = s;    array[ 9] = 0.0f; array[10] = c;    array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：z 軸中心の回転変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadRotateZ(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = c;    array[ 1] = s;    array[ 2] = 0.0f; array[ 3] = 0.0f;
  array[ 4] = -s;   array[ 5] = c;    array[ 6] = 0.0f; array[ 7] = 0.0f;
  array[ 8] = 0.0f; array[ 9] = 0.0f; array[10] = 1.0f; array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：任意軸中心の回転変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  GLfloat d = sqrt(x * x + y * y + z * z);
  
  if (d > 0.0f)
  {
    GLfloat l  = x / d, m  = y / d, n  = z / d;
    GLfloat l2 = l * l, m2 = m * m, n2 = n * n;
    GLfloat lm = l * m, mn = m * n, nl = n * l;
    GLfloat c = cos(a), c1 = 1.0f - c;
    GLfloat s = sin(a);
    
    array[ 0] = (1.0f - l2) * c + l2;
    array[ 1] = lm * c1 + n * s;
    array[ 2] = nl * c1 - m * s;
    array[ 3] = 0.0f;
    
    array[ 4] = lm * c1 - n * s;
    array[ 5] = (1.0f - m2) * c + m2;
    array[ 6] = mn * c1 + l * s;
    array[ 7] = 0.0f;
    
    array[ 8] = nl * c1 + m * s;
    array[ 9] = mn * c1 - l * s;
    array[10] = (1.0f - n2) * c + n2;
    array[11] = 0.0f;
    
    array[12] = 0.0f;
    array[13] = 0.0f;
    array[14] = 0.0f;
    array[15] = 1.0f;
  }
  
  return *this;
}

/*
** 変換行列：転置行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadTranspose(const GgMatrix &m)
{
  array[ 0] = m.array[ 0];
  array[ 1] = m.array[ 4];
  array[ 2] = m.array[ 8];
  array[ 3] = m.array[12];
  array[ 4] = m.array[ 1];
  array[ 5] = m.array[ 5];
  array[ 6] = m.array[ 9];
  array[ 7] = m.array[13];
  array[ 8] = m.array[ 2];
  array[ 9] = m.array[ 6];
  array[10] = m.array[10];
  array[11] = m.array[14];
  array[12] = m.array[ 3];
  array[13] = m.array[ 7];
  array[14] = m.array[11];
  array[15] = m.array[15];
  
  return *this;
}

/*
** 変換行列：逆行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadInvert(const GgMatrix &m)
{
  GLfloat lu[20], *plu[4];
  const GLfloat *marray = m.array;
  
  // j 行の要素の値の絶対値の最大値を plu[j][4] に求める
  for (int j = 0; j < 4; ++j)
  {
    GLfloat max = fabs(*(plu[j] = lu + 5 * j) = *(marray++));
    
    for (int i = 0; ++i < 4;)
    {
      GLfloat a = fabs(plu[j][i] = *(marray++));
      if (a > max) max = a;
    }
    if (max == 0.0f) return *this;
    plu[j][4] = 1.0f / max;
  }
  
  // ピボットを考慮した LU 分解
  for (int j = 0; j < 4; ++j)
  {
    GLfloat max = fabs(plu[j][j] * plu[j][4]);
    int i = j;
    
    for (int k = j; ++k < 4;)
    {
      GLfloat a = fabs(plu[k][j] * plu[k][4]);
      if (a > max)
      {
        max = a;
        i = k;
      }
    }
    if (i > j)
    {
      GLfloat *t = plu[j];
      plu[j] = plu[i];
      plu[i] = t;
    }
    if (plu[j][j] == 0.0f) return *this;
    for (int k = j; ++k < 4;)
    {
      plu[k][j] /= plu[j][j];
      for (int i = j; ++i < 4;)
      {
        plu[k][i] -= plu[j][i] * plu[k][j];
      }
    }
  }
  
  // LU 分解から逆行列を求める
  for (int k = 0; k < 4; ++k)
  {
    // array に単位行列を設定する
    for (int i = 0; i < 4; ++i)
    {
      array[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0f : 0.0f;
    }
    // lu から逆行列を求める
    for (int i = 0; i < 4; ++i)
    {
      for (int j = i; ++j < 4;)
      {
        array[j * 4 + k] -= array[i * 4 + k] * plu[j][i];
      }
    }
    for (int i = 4; --i >= 0;)
    {
      for (int j = i; ++j < 4;)
      {
        array[i * 4 + k] -= plu[i][j] * array[j * 4 + k];
      }
      array[i * 4 + k] /= plu[i][i];
    }
  }
  
  return *this;
}

/*
** 変換行列：法線変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadNormal(const GgMatrix &m)
{
  array[ 0] = m.array[ 5] * m.array[10] - m.array[ 6] * m.array[ 9];
  array[ 1] = m.array[ 6] * m.array[ 8] - m.array[ 4] * m.array[10];
  array[ 2] = m.array[ 4] * m.array[ 9] - m.array[ 5] * m.array[ 8];
  array[ 4] = m.array[ 9] * m.array[ 2] - m.array[10] * m.array[ 1];
  array[ 5] = m.array[10] * m.array[ 0] - m.array[ 8] * m.array[ 2];
  array[ 6] = m.array[ 8] * m.array[ 1] - m.array[ 9] * m.array[ 0];
  array[ 8] = m.array[ 1] * m.array[ 6] - m.array[ 2] * m.array[ 5];
  array[ 9] = m.array[ 2] * m.array[ 4] - m.array[ 0] * m.array[ 6];
  array[10] = m.array[ 0] * m.array[ 5] - m.array[ 1] * m.array[ 4];
  array[ 3] = array[ 7] = array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：視野変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  GLfloat l;
  
  // z 軸 = e - t
  tx = ex - tx;
  ty = ey - ty;
  tz = ez - tz;
  l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 2] = tx / l;
  array[ 6] = ty / l;
  array[10] = tz / l;
  
  // x 軸 = u x z 軸
  tx = uy * array[10] - uz * array[ 6];
  ty = uz * array[ 2] - ux * array[10];
  tz = ux * array[ 6] - uy * array[ 2];
	l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 0] = tx / l;
  array[ 4] = ty / l;
  array[ 8] = tz / l;
  
  // y 軸 = z 軸 x x 軸
  array[ 1] = array[ 6] * array[ 8] - array[10] * array[ 4];
  array[ 5] = array[10] * array[ 0] - array[ 2] * array[ 8];
  array[ 9] = array[ 2] * array[ 4] - array[ 6] * array[ 0];
  
  // 平行移動
  array[12] = -(ex * array[ 0] + ey * array[ 4] + ez * array[ 8]);
  array[13] = -(ex * array[ 1] + ey * array[ 5] + ez * array[ 9]);
  array[14] = -(ex * array[ 2] + ey * array[ 6] + ez * array[10]);
  
  // 残り
  array[ 3] = array[ 7] = array[11] = 0.0f;
  array[15] = 1.0f;
  
  return *this;
}

/*
** 変換行列：平行投影変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    array[ 0] =  2.0f / dx;
    array[ 5] =  2.0f / dy;
    array[10] = -2.0f / dz;
    array[12] = -(right + left) / dx;
    array[13] = -(top + bottom) / dy;
    array[14] = -(zFar + zNear) / dz;
    array[15] =  1.0f;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] = array[11] = 0.0f;
  }
  
  return *this;
}

/*
** 変換行列：透視投影変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    array[ 0] =  2.0f * zNear / dx;
    array[ 5] =  2.0f * zNear / dy;
    array[ 8] =  (right + left) / dx;
    array[ 9] =  (top + bottom) / dy;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[12] = array[13] = array[15] = 0.0f;
  }
  
  return *this;
}

/*
** 変換行列：画角から透視投影変換行列を設定する
*/
gg::GgMatrix &gg::GgMatrix::loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  GLfloat dz = zFar - zNear;
  
  if (dz != 0.0f)
  {
    GLfloat f = 1.0f / tan(fovy * 0.5f);
    
    array[ 0] = f / aspect;
    array[ 5] = f;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] =
    array[12] = array[13] = array[15] = 0.0f;
  }
  
  return *this;
}

/*
** 変換行列：視野変換行列を乗じる（視点の移動）
*/
gg::GgMatrix &gg::GgMatrix::lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  GgMatrix m;
  
  m.loadLookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
  multiply(m);
  
  return *this;
}

/*
** 変換行列：平行投影変換行列を乗じる
*/
gg::GgMatrix &gg::GgMatrix::orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadOrthogonal(left, right, bottom, top, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** 変換行列：透視投影変換行列を乗じる
*/
gg::GgMatrix &gg::GgMatrix::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadFrustum(left, right, bottom, top, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** 変換行列：画角から求めた透視投影変換行列を乗じる
*/
gg::GgMatrix &gg::GgMatrix::perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadPerspective(fovy, aspect, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** 四元数：四元数 p, q の和を r に求める
*/
void gg::GgQuaternion::add(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[0] + q[0];
  r[1] = p[1] + q[1];
  r[2] = p[2] + q[2];
  r[3] = p[3] + q[3];
}

/*
** 四元数：四元数 p, q の差を r に求める
*/
void gg::GgQuaternion::subtract(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[0] - q[0];
  r[1] = p[1] - q[1];
  r[2] = p[2] - q[2];
  r[3] = p[3] - q[3];
}

/*
** 四元数：四元数 p, q の積を r に求める
*/
void gg::GgQuaternion::multiply(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[1] * q[2] - p[2] * q[1] + p[0] * q[3] + p[3] * q[0];
  r[1] = p[2] * q[0] - p[0] * q[2] + p[1] * q[3] + p[3] * q[1];
  r[2] = p[0] * q[1] - p[1] * q[0] + p[2] * q[3] + p[3] * q[2];
  r[3] = p[3] * q[3] - p[0] * q[0] - p[1] * q[1] - p[2] * q[2];
}

/*
** 四元数：四元数 q が表す変換行列を m に求める
*/
void gg::GgQuaternion::toMatrix(GLfloat *m, const GLfloat *q) const
{
  GLfloat xx = q[0] * q[0] * 2.0f;
  GLfloat yy = q[1] * q[1] * 2.0f;
  GLfloat zz = q[2] * q[2] * 2.0f;
  GLfloat xy = q[0] * q[1] * 2.0f;
  GLfloat yz = q[1] * q[2] * 2.0f;
  GLfloat zx = q[2] * q[0] * 2.0f;
  GLfloat xw = q[0] * q[3] * 2.0f;
  GLfloat yw = q[1] * q[3] * 2.0f;
  GLfloat zw = q[2] * q[3] * 2.0f;
  
  m[ 0] = 1.0f - yy - zz;
  m[ 1] = xy + zw;
  m[ 2] = zx - yw;
  m[ 4] = xy - zw;
  m[ 5] = 1.0f - zz - xx;
  m[ 6] = yz + xw;
  m[ 8] = zx + yw;
  m[ 9] = yz - xw;
  m[10] = 1.0f - xx - yy;
  m[ 3] = m[ 7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
  m[15] = 1.0f;
}

/*
** 四元数：回転変換行列 m が表す四元数を q に求める
*/
void gg::GgQuaternion::toQuaternion(GLfloat *q, const GLfloat *m) const
{
  GLfloat tr = m[0] + m[5] + m[10] + m[15];
  
  if (tr > 0.0f)
  {
    q[3] = sqrt(tr) * 0.5f;
    q[0] = (m[6] - m[9]) * 0.25f / q[3];
    q[1] = (m[8] - m[2]) * 0.25f / q[3];
    q[2] = (m[1] - m[4]) * 0.25f / q[3];
  }
}

/*
** 四元数：球面線形補間 p に q と r を t で補間した四元数を求める
*/
void gg::GgQuaternion::slerp(GLfloat *p, const GLfloat *q, const GLfloat *r, GLfloat t) const
{
  GLfloat qr = q[0] * r[0] + q[1] * r[1] + q[2] * r[2] + q[3] * r[3];
  GLfloat ss = 1.0f - qr * qr;
  
  if (ss == 0.0f)
  {
    if (p != q)
    {
      p[0] = q[0];
      p[1] = q[1];
      p[2] = q[2];
      p[3] = q[3];
    }
  }
  else
  {
    GLfloat sp = sqrt(ss);
    GLfloat ph = acos(qr);
    GLfloat pt = ph * t;
    GLfloat t1 = sin(pt) / sp;
    GLfloat t0 = sin(ph - pt) / sp;
    
    p[0] = q[0] * t0 + r[0] * t1;
    p[1] = q[1] * t0 + r[1] * t1;
    p[2] = q[2] * t0 + r[2] * t1;
    p[3] = q[3] * t0 + r[3] * t1;
  }
}

/*
** 四元数：(x, y, z) を軸とし角度 a 回転する四元数を求める
*/
gg::GgQuaternion &gg::GgQuaternion::loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  GLfloat l = x * x + y * y + z * z;
  
  if (l != 0.0)
  {
    GLfloat s = sin(a *= 0.5f) / sqrt(l);
    
    array[0] = x * s;
    array[1] = y * s;
    array[2] = z * s;
  }
  else
  {
    array[0] = array[1] = array[2] = 0.0f;
  }
  array[3] = cos(a);
  
  return *this;
}

/*
** 四元数：オイラー角 (h, p, r) にもとづいて四元数を求める
*/
gg::GgQuaternion &gg::GgQuaternion::loadEuler(GLfloat h, GLfloat p, GLfloat r)
{
  return loadRotate(-sin(h) * cos(p), sin(p), cos(h) * cos(p), r);
}

/*
** 四元数：ノルム
*/
GLfloat gg::GgQuaternion::norm(void) const
{
  return sqrt(array[0] * array[0] + array[1] * array[1] + array[2] * array[2] + array[3] * array[3]);
}

/*
** 四元数：四元数 q の共役を r に求める
*/
gg::GgQuaternion gg::GgQuaternion::conjugate(void) const
{
  GgQuaternion t(-this->array[0], -this->array[1], -this->array[2], this->array[3]);
  
  return t;
}

/*
** 四元数：四元数 q の逆を r に求める
*/
gg::GgQuaternion gg::GgQuaternion::invert(void) const
{
  GgQuaternion t = this->conjugate();
  GLfloat l = this->array[0] * this->array[0] + this->array[1] * this->array[1]
  + this->array[2] * this->array[2] + this->array[3] * this->array[3];
  
  if (l > 0.0f)
  {
    t.array[0] /= l;
    t.array[1] /= l;
    t.array[2] /= l;
    t.array[3] /= l;
  }
  
  return t;
}

/*
** 四元数：正規化
*/
gg::GgQuaternion gg::GgQuaternion::normalize(void) const
{
  GgQuaternion t = *this;
  GLfloat l = this->norm();
  
  if (l > 0.0f)
  {
    t.array[0] /= l;
    t.array[1] /= l;
    t.array[2] /= l;
    t.array[3] /= l;
  }
  
  return t;
}

/*
** 簡易トラックボール処理：コンストラクタ
*/
gg::GgTrackball::GgTrackball(void)
{
  // ドラッグ中ではない
  drag = false;
  
  // 単位クォーターニオン
  cq.loadIdentity();
  
  // 回転行列の初期化
  cq.getMatrix(rt);
}

/*
** 簡易トラックボール処理：トラックボールする領域の設定
**
**    Reshape コールバック (resize) の中で実行する
**    (w, h): ウィンドウサイズ
*/
void gg::GgTrackball::region(int w, int h)
{
  // マウスポインタ位置のウィンドウ内の相対的位置への換算用
  sx = 1.0f / (float)w;
  sy = 1.0f / (float)h;
}

/*
** 簡易トラックボール処理：ドラッグ開始時の処理
**
**    マウスボタンを押したときに実行する
**    (x, y): 現在のマウス位置
*/
void gg::GgTrackball::start(int x, int y)
{
  // ドラッグ開始
  drag = true;
  
  // ドラッグ開始点を記録する
  cx = x;
  cy = y;
}

/*
** 簡易トラックボール処理：ドラッグ中の処理
**
**    マウスのドラッグ中に実行する
**    (x, y): 現在のマウス位置
*/
void gg::GgTrackball::motion(int x, int y)
{
  if (drag)
  {
    float dx, dy, a;
    
    // マウスポインタの位置のドラッグ開始位置からの変位
    dx = (x - cx) * sx;
    dy = (y - cy) * sy;
    
    // マウスポインタの位置のドラッグ開始位置からの距離
    a = sqrt(dx * dx + dy * dy);
    
    if (a != 0.0)
    {
      // 回転軸と回転角から四元数を作る
      GgQuaternion dq;
      dq.loadRotate(dy, dx, 0.0f, a * 6.283185f);
      
      // 現在の回転の四元数に作った四元数を掛けて合成する
      tq = dq * cq;
      
      // 合成した四元数から回転の変換行列を求める
      tq.getMatrix(rt);
    }
  }
}

/*
** 簡易トラックボール処理：停止時の処理
**
**    マウスボタンを離したときに実行する
**    (x, y): 現在のマウス位置
*/
void gg::GgTrackball::stop(int x, int y)
{
  // ドラッグ終了点における回転を求める
  motion(x, y);
  
  // 現在の回転を表す四元数を正規化して保存する
  cq = tq.normalize();
  
  // ドラッグ終了
  drag = false;
}

/*
** 点群：コピー
*/
void gg::GgPoints::copy(const GgPoints &o)
{
  if (o.nv > 0)
  {
    // メモリの確保
    GLfloat (*vert)[3] = new GLfloat[o.nv][3];
    
    // データのコピー
    memcpy(vert, o.vert, sizeof (GLfloat[3]) * o.nv);
    
    // データの登録
    entry(o.nv, vert);
  }
}

/*
** 点群：描画
*/
void gg::GgPoints::draw(void) const
{
  // シェーダーの選択
  getShader()->use(vert);
  
  // 頂点配列で図形を描画する
  glDrawArrays(GL_POINTS, 0, nv);
}

/*
** メッシュ：コピー
*/
void gg::GgPolygon::copy(const GgPolygon &o)
{
  if (o.getNv() > 0)
  {
    // メモリの確保
    GLfloat (*norm)[3] = new GLfloat[o.getNv()][3];
    
    // データのコピー
    memcpy(norm, o.norm, sizeof (GLfloat[3]) * o.getNv());
    
    // データの登録
    entry(norm);
  }
}

/*
** メッシュ：描画
*/
void gg::GgPolygon::draw(void) const
{
  // シェーダーの選択
  getShader()->use(getVert(), norm);
  
  // 頂点配列で図形を描画する
  glDrawArrays(GL_TRIANGLE_FAN, 0, getNv());
}

/*
** オブジェクト：コピー
*/
void gg::GgObject::copy(const GgObject &o)
{
  if (o.nf > 0)
  {
    // メモリの確保
    GLuint (*face)[3] = new GLuint[o.nf][3];
    
    // データのコピー
    memcpy(face, o.face, sizeof (GLuint[3]) * o.nf);
    
    // データの登録
    entry(o.nf, face);
  }
}

/*
** オブジェクト：描画
*/
void gg::GgObject::draw(void) const
{
  // シェーダプログラムの選択
  getShader()->use(getVert(), getNorm());
  
  // 頂点のインデックスの場所を指定して図形を描画する
  glDrawElements(GL_TRIANGLES, getNf() * 3, GL_UNSIGNED_INT, getFace());
}

/*
** 球状の点群
*/
gg::GgPoints *gg::ggPointSphere(GLuint nv, GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius)
{
  // メモリの確保
  GLfloat (*vert)[3] = new GLfloat[nv][3];
  
  // 点の生成
  for (GLuint v = 0; v < nv; ++v)
  {
    float r = radius * (float)rand() / (float)RAND_MAX;
    float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
    float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
    float sp = sqrt(1.0f - cp * cp);
    float ct = cos(t), st = sin(t);
    
    vert[v][0] = r * sp * ct + cx;
    vert[v][1] = r * sp * st + cy;
    vert[v][2] = r * cp + cz;
  }
  
  return new gg::GgPoints(nv, vert);
}

/*
** 矩形
*/
gg::GgPolygon *gg::ggRectangle(GLfloat width, GLfloat height)
{
  // 基準となる形状
  static const GLfloat p[][2] =
  {
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
  };
  
  // メモリの確保
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  try
  {
    vert = new GLfloat[4][3];
    norm = new GLfloat[4][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    throw e;
  }

  for (int v = 0; v < 4; ++v)
  {
    vert[v][0] = p[v][0] * width;
    vert[v][1] = p[v][1] * height;
    vert[v][2] = 0.0f;
    
    norm[v][0] = 0.0f;
    norm[v][1] = 0.0f;
    norm[v][2] = 1.0f;
  }
  
  return new gg::GgPolygon(4, vert, norm);
}

/*
** 楕円
*/
gg::GgPolygon *gg::ggEllipse(GLfloat width, GLfloat height, GLuint slices)
{
  // メモリの確保
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  try
  {
    vert = new GLfloat[slices][3];
    norm = new GLfloat[slices][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    throw e;
  }
  
  for (GLuint v = 0; v < slices; ++v)
  {
    float t = 6.2831853f * (float)v / (float)slices;
    
    vert[v][0] = cos(t) * width * 0.5f;
    vert[v][1] = sin(t) * height * 0.5f;
    vert[v][2] = 0.0f;
    
    norm[v][0] = 0.0f;
    norm[v][1] = 0.0f;
    norm[v][2] = 1.0f;
  }
  
  return new gg::GgPolygon(slices, vert, norm);
}

/*
** 三角形分割された Alias OBJ ファイル
*/
gg::GgObject *gg::ggObj(const char *name, bool normalize)
{
  // ファイルの読み込み
  std::ifstream file(name, std::ios::binary);
  if (file.fail())
  {
    std::cerr << "Can't open file: " << name << std::endl;
    return 0;
  }
  
  // データの数と座標値の最小値・最大値を調べる
  char buf[1024];
  int nv, nf;
  nv = nf = 0;
  float xmin, xmax, ymin, ymax, zmin, zmax;
  xmax = ymax = zmax = -(xmin = ymin = zmin = FLT_MAX);
  while (file.getline(buf, sizeof buf))
  {
    if (buf[0] == 'v' && buf[1] == ' ')
    {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      if (x < xmin) xmin = x;
      if (x > xmax) xmax = x;
      if (y < ymin) ymin = y;
      if (y > ymax) ymax = y;
      if (z < zmin) zmin = z;
      if (z > zmax) zmax = z;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ')
    {
      ++nf;
    }
  }
  
  // メモリの確保
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  GLfloat (*fnorm)[3] = 0;
  GLuint (*face)[3] = 0;
  try
  {
    vert = new GLfloat[nv][3];
    norm = new GLfloat[nv][3];
    fnorm = new GLfloat[nf][3];
    face = new GLuint[nf][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    delete[] fnorm;
    delete[] face;
    throw e;
  }
  
  // 正規化
  GLfloat scale, cx, cy, cz;
  if (normalize)
  {
    float sx = xmax - xmin;
    float sy = ymax - ymin;
    float sz = zmax - zmin;
    scale = sx;
    if (sy > scale) scale = sy;
    if (sz > scale) scale = sz;
    scale = (scale != 0.0f) ? 1.0f / scale : 1.0f;
    cx = (xmax + xmin) * 0.5f;
    cy = (ymax + ymin) * 0.5f;
    cz = (zmax + zmin) * 0.5f;
  }    
  else {
    scale = 1.0f;
    cx = cy = cz = 0.0f;
  }
  
  // ファイルの巻き戻し
  file.clear();
  file.seekg(0L, std::ios::beg);
  
  // データの読み込み
  nv = nf = 0;
  while (file.getline(buf, sizeof buf))
  {
    if (buf[0] == 'v' && buf[1] == ' ')
    {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      vert[nv][0] = (x - cx) * scale;
      vert[nv][1] = (y - cy) * scale;
      vert[nv][2] = (z - cz) * scale;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ')
    {
      if (sscanf(buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3)
        if (sscanf(buf + 2, "%d//%*d %d//%*d %d//%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3)
          sscanf(buf + 2, "%d %d %d", face[nf], face[nf] + 1, face[nf] + 2);
      --face[nf][0];
      --face[nf][1];
      --face[nf][2];
      ++nf;
    }
  }
  
  // 面法線ベクトルの算出
  for (int f = 0; f < nf; ++f)
  {
    GLfloat dx1 = vert[face[f][1]][0] - vert[face[f][0]][0];
    GLfloat dy1 = vert[face[f][1]][1] - vert[face[f][0]][1];
    GLfloat dz1 = vert[face[f][1]][2] - vert[face[f][0]][2];
    GLfloat dx2 = vert[face[f][2]][0] - vert[face[f][0]][0];
    GLfloat dy2 = vert[face[f][2]][1] - vert[face[f][0]][1];
    GLfloat dz2 = vert[face[f][2]][2] - vert[face[f][0]][2];
    
    // 外積
    fnorm[f][0] = dy1 * dz2 - dz1 * dy2;
    fnorm[f][1] = dz1 * dx2 - dx1 * dz2;
    fnorm[f][2] = dx1 * dy2 - dy1 * dx2;
  }
  
  // 頂点の法線ベクトルの値を 0 にしておく
  for (int v = 0; v < nv; ++v)
    norm[v][0] = norm[v][1] = norm[v][2] = 0.0f;
  
  // 面の法線ベクトルを頂点の法線ベクトルに積算する
  for (int f = 0; f < nf; ++f)
  {
    norm[face[f][0]][0] += fnorm[f][0];
    norm[face[f][0]][1] += fnorm[f][1];
    norm[face[f][0]][2] += fnorm[f][2];
    
    norm[face[f][1]][0] += fnorm[f][0];
    norm[face[f][1]][1] += fnorm[f][1];
    norm[face[f][1]][2] += fnorm[f][2];
    
    norm[face[f][2]][0] += fnorm[f][0];
    norm[face[f][2]][1] += fnorm[f][1];
    norm[face[f][2]][2] += fnorm[f][2];
  }
  
  // 頂点の法線ベクトルを正規化する
  for (int v = 0; v < nv; ++v)
  {
    GLfloat a = sqrt(norm[v][0] * norm[v][0]
                   + norm[v][1] * norm[v][1]
                   + norm[v][2] * norm[v][2]);
    
    if (a != 0.0)
    {
      norm[v][0] /= a;
      norm[v][1] /= a;
      norm[v][2] /= a;
    }
  }
  
  // 面の法線ベクトルは頂点の法線ベクトルの算出にしか使わないので解放
  delete[] fnorm;
  
  return new gg::GgObject(nv, vert, norm, nf, face);
}
