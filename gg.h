/*
** ゲームグラフィックス特論用補助プログラム
**

Copyright (c) 2011 Kohe Tokoi. All Rights Reserved.

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

#ifndef __GG_H__
#define __GG_H__

#if defined(WIN32)
#  pragma warning(disable:4996)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  pragma comment(lib, "glew32.lib")
#  include "glew.h"
#  include "wglew.h"
#  include "glut.h"
#elif defined(X11)
#  define GL_GLEXT_PROTOTYPES
#  define GLX_GLXEXT_PROTOTYPES
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glx.h>
#  include <GL/glut.h>
#elif defined(__APPLE__)
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
// GL_ARB_texture_float
#  define GL_RGBA32F GL_RGBA32F_ARB
#  define GL_RGB32F GL_RGB32F_ARB
#  define GL_ALPHA32F GL_ALPHA32F_ARB
#  define GL_INTENSITY32F GL_INTENSITY32F_ARB
#  define GL_LUMINANCE32F GL_LUMINANCE32F_ARB
#  define GL_LUMINANCE_ALPHA32F GL_LUMINANCE_ALPHA32F_ARB
#  define GL_RGBA16F GL_RGBA16F_ARB
#  define GL_RGB16F GL_RGB16F_ARB
#  define GL_ALPHA16F GL_ALPHA16F_ARB
#  define GL_INTENSITY16F GL_INTENSITY16F_ARB
#  define GL_LUMINANCE16F GL_LUMINANCE16F_ARB
#  define GL_LUMINANCE_ALPHA16F GL_LUMINANCE_ALPHA16F_ARB
// GL_EXT_transform_feedback
#  define glBindBufferRange glBindBufferRangeEXT
#  define glBindBufferOffset glBindBufferOffsetEXT
#  define glBindBufferBase glBindBufferBaseEXT
#  define glBeginTransformFeedback glBeginTransformFeedbackEXT
#  define glEndTransformFeedback glEndTransformFeedbackEXT
#  define glTransformFeedbackVaryings glTransformFeedbackVaryingsEXT
#  define glGetTransformFeedbackVarying glGetTransformFeedbackVaryingEXT
#  define glGetIntegerIndexedv glGetIntegerIndexedvEXT
#  define glGetBooleanIndexedv glGetBooleanIndexedvEXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER GL_TRANSFORM_FEEDBACK_BUFFER_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_START GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT
#  define GL_INTERLEAVED_ATTRIBS GL_INTERLEAVED_ATTRIBS_EXT
#  define GL_SEPARATE_ATTRIBS GL_SEPARATE_ATTRIBS_EXT
#  define GL_PRIMITIVES_GENERATED GL_PRIMITIVES_GENERATED_EXT
#  define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT
#  define GL_RASTERIZER_DISCARD GL_RASTERIZER_DISCARD_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYINGS GL_TRANSFORM_FEEDBACK_VARYINGS_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_MODE GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT
// GL_APPLE_vertex_array_object
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#  define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#  define glIsVertexArray glIsVertexArrayAPPLE
#  define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_APPLE
#else
#  error "This platform is not supported."
#endif

#define GG_DEG_TO_RAD(a) ((a) * 0.017453293f)

namespace gg
{
  /*
  ** ゲームグラフィックス特論の都合にもとづく初期化
  **
  **    OpenGL のレンダリングコンテキストを作成後に実行する
  */
  extern void ggInit(void);

  /*
  ** OpenGL のエラーチェック
  **
  **    OpenGL の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **    msg はメッセージに乗じる文字列
  */
  extern void ggError(const char *msg = 0);

  /*
  ** FBO のエラーチェック
  **
  **    FBO の API を呼び出し直後に実行すればエラーのあるときにメッセージを表示する
  **    msg はメッセージに乗じる文字列
  */
  extern void ggFBOError(const char *msg = 0);

  /*
  ** シェーダーソースファイルの読み込み
  */
  extern GLuint loadShader(
    const char *vert,                   // バーテックスシェーダのソースファイル名
    const char *frag = 0,               // フラグメントシェーダのソースファイル名（0 なら不使用）
    const char *geom = 0,               // ジオメトリシェーダのソースファイル名（0 なら不使用）
    GLenum input = GL_TRIANGLES,        // ジオメトリシェーダの入力プリミティブ
    GLenum output = GL_TRIANGLE_STRIP,  // ジオメトリシェーダの出力プリミティブ
    int nvarying = 0,                   // フィードバックする varying 変数の数（0 なら不使用）
    const char **varyings = 0           // フィードバックする varying 変数のリスト
  );

  /*
  ** テクスチャマッピング用の RAW 画像ファイルの読み込み
  */
  extern void loadImage(const char *name, int width, int height, GLenum format);

  /*
  ** 高さマップ用の RAW 画像ファイルの読み込んで法線マップを作成する
  */
  extern void loadHeight(const char *name, int width, int height, float nz);
}

#endif
