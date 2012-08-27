#ifndef __GG_BLURSHADER_H__
#define __GG_BLURSHADER_H__

/*
** モーションブラー
*/
#include "GgSimpleShader.h"

namespace gg
{
  class GgBlurShader
    : public GgSimpleShader
  {
    // feedback buffer の varying 変数名
    static const char *varyings[];
    
    // バッファオブジェクト
    struct
    {
      // feedback buffer に使うバッファオブジェクト
      GgBuffer<GLfloat[3]> fb[2];

      // feedback buffer の選択
      unsigned int buffer;
    } b;
    
    // 場所
    struct
    {
      GLint p0;         // フィードバックバッファからの読み出し用の attribute 変数の場所
    } loc;
    
  public:
    
    // デストラクタ
    virtual ~GgBlurShader() {}
    
    // コンストラクタ
    GgBlurShader(void) {}
    GgBlurShader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0);
    GgBlurShader(const GgBlurShader &o)
      : GgSimpleShader(o), b(o.b), loc(o.loc) {}
    
    // 代入
    GgBlurShader &operator=(const GgBlurShader &o)
    {
      if (&o != this)
      {
        GgSimpleShader::operator=(o);
        b = o.b;
        loc = o.loc;
      }
      return *this;
    }

    // シェーダの使用開始
    virtual void use(GLuint vert, ...) const;
    
    // シェーダの使用終了
    virtual void unuse(void) const;
    
    // バッファオブジェクトの複写
    void copyBuffer(GLuint num, GLuint buf);
    
    // フィードバックバッファオブジェクトの選択
    void selectBuffer(unsigned int frame)
    {
      b.buffer = frame & 1;
    }
    
    // フィードバックバッファオブジェクトの選択
    void swapBuffers(void)
    {
      b.buffer = 1 - b.buffer;
    }
  };
}

#endif
