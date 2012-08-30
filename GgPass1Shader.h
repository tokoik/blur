#ifndef __GG_Pass1Shader_H__
#define __GG_Pass1Shader_H__

/*
** モーションブラー (Pass 1)
*/
#include "GgSimpleShader.h"

namespace gg
{
  class GgPass1Shader
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
      unsigned int select;
    } b;
    
  public:
    
    // デストラクタ
    virtual ~GgPass1Shader(void) {}
    
    // コンストラクタ
    GgPass1Shader(void) {}
    GgPass1Shader(const char *vert, const char *frag = 0,
      const char *geom = 0, GLenum input = GL_TRIANGLES, GLenum output = GL_TRIANGLE_STRIP, GLint vertices = 0)
      : GgSimpleShader(vert, frag, geom, input, output, vertices, 1, varyings)
    {
      // 最初に使用するバッファ
      b.select = 0;
    }
    GgPass1Shader(const GgPass1Shader &o)
      : GgSimpleShader(o), b(o.b) {}
    
    // 代入
    GgPass1Shader &operator=(const GgPass1Shader &o)
    {
      if (&o != this)
      {
        GgSimpleShader::operator=(o);
        b = o.b;
      }
      return *this;
    }

    // シェーダの使用開始
    virtual void use(GLuint vert, ...) const;
    
    // シェーダの使用終了
    virtual void unuse(void) const;
    
    // バッファオブジェクト確保と値の設定
    void copyBuffer(GLuint num, GLuint buf);
    
    // フィードバックバッファオブジェクトの選択
    void selectBuffer(unsigned int frame)
    {
      b.select = frame & 1;
    }
    
    // フィードバックバッファオブジェクトの選択
    void swapBuffers(void)
    {
      b.select = 1 - b.select;
    }

    // フロントバッファのバッファオブジェクト
    GLuint front(void)
    {
      return b.fb[b.select].buf();
    }

    // バックバッファのバッファオブジェクト
    GLuint back(void)
    {
      return b.fb[1 - b.select].buf();
    }
  };
}

#endif
