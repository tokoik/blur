#include <cmath>
#include <cstdlib>
#include <cstring>

#include "gg.h"
using namespace gg;

/*
** トラックボール処理
*/
static GgTrackball tb;

/*
** 変換行列
*/
static GgMatrix mv;   // 視野変換行列
static GgMatrix mp;   // 投影変換行列
static GgMatrix mt;   // 平行移動

/*
** シェーダ
*/
#include "GgPass1Shader.h"
#include "GgPass2Shader.h"
static GgPass1Shader *pass1 = 0;
static GgPass2Shader *pass2 = 0;

/*
** OBJ ファイル
*/
static GgTriangles *model = 0;

/*
** 画面いっぱいの矩形
*/
static GgTriangles *rect = 0;

/*
** テクスチャ
*/
static GgTexture *texture0 = 0;
static GgTexture *texture1 = 0;

/*
** フレームバッファオブジェクト
*/
#define FBOWIDTH 1024
#define FBOHEIGHT 1024
static GLuint fb;

/*
** ビューポート
*/
static int vp[4];

static void display(void)
{
  // レンダーターゲットのリスト
  static const GLenum bufs[] =
  {
    GL_COLOR_ATTACHMENT0_EXT, //   色
    GL_COLOR_ATTACHMENT1_EXT, //   速度
  };
  
  // フレームバッファオブジェクト指定
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  // レンダーターゲット指定
  glDrawBuffers(sizeof bufs / sizeof bufs[0], bufs);
  
  // ビューポートの設定
  glViewport(0, 0, FBOWIDTH, FBOHEIGHT);
  
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // レンダリング
  glEnable(GL_DEPTH_TEST);
  pass1->loadMatrix(mp, mv * mt * tb.get());
  model->draw();
  pass1->swapBuffers();
  
  // フレームバッファオブジェクト解除
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // レンダーターゲット復帰
  glDrawBuffer(GL_BACK);
  
  // カラーテクスチャの使用
  texture0->use(0);
  texture1->use(1);

  // 画面いっぱいの矩形の描画
  glViewport(vp[0], vp[1], vp[2], vp[3]);
  glDisable(GL_DEPTH_TEST);
  rect->draw();

  // カラーテクスチャの解放
  texture0->unuse();
  texture1->unuse();

  // ダブルバッファリング
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // ウィンドウ全体に表示
  glViewport(vp[0] = 0, vp[1] = 0, vp[2] = w, vp[3] = h);
  
  // 投影変換行列
  mp.loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);

  // トラックボールする範囲
  tb.region(w, h);
  
  // Pass 2 シェーダの乱数テーブルの生成
  pass2->size(1.0f / (GLfloat)w, 1.0f / (GLfloat)h);
}

static void idle(void)
{
  // 画面の描き替え
  glutPostRedisplay();
}

static void pick(GLfloat *pw, GLint x, GLint y, GLfloat z, const GgMatrix &mc)
{
  // クリッピング空間中の座標値
  GLfloat pc[] =
  {
    (GLfloat)x * 2.0f / (GLfloat)FBOWIDTH -  1.0f,
    (GLfloat)y * 2.0f / (GLfloat)FBOHEIGHT - 1.0f,
    z * 2.0f - 1.0f,
    1.0f,
  };

  // ワールド空間中の座標値
  mc.projection(pw, pc);
}

static GLfloat p0[4], z0;
static GgMatrix mt0, imc;

// 押されているボタン
static int press = -1;

static void mouse(int button, int state, int x, int y)
{
  press = button;
  
  if (state == GLUT_DOWN)
  {
    // ビューポート上のクリック位置に直す
    GLint x0 = x - vp[0];
    GLint y0 = vp[3] - vp[1] - y;

    // フレームバッファオブジェクト上のクリック位置に直す
    x0 = (GLint)((GLfloat)x0 * (GLfloat)FBOWIDTH / (GLfloat)vp[2] + 0.5f);
    y0 = (GLint)((GLfloat)y0 * (GLfloat)FBOHEIGHT / (GLfloat)vp[3] + 0.5f);

    // クリックしたところの深度値を読む
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    glReadPixels(x0, y0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // 背景でないとき
    if (z0 < 1.0f)
    {
      switch (press)
      {
      case GLUT_LEFT_BUTTON:
        // 平行移動開始
        imc.loadInvert(mp * mv);
        pick(p0, x0, y0, z0, imc);
        mt0 = mt;
        break;
      case GLUT_RIGHT_BUTTON:
        // トラックボール開始
        tb.start(x, y);
        break;
      default:
        break;
      }
      
      // アニメーション開始
      glutIdleFunc(idle);
    }
    else
    {
      // クリックしなかったことにする
      press = -1;
    }
  }
  else
  {
    switch (press)
    {
    case GLUT_LEFT_BUTTON:
      // 平行移動終了
      break;
    case GLUT_RIGHT_BUTTON:
      // トラックボール終了
      tb.stop(x, y);
      break;
    default:
      break;
    }
    
    // アニメーション停止
    glutIdleFunc(0);
  }
}

static void motion(int x, int y)
{
  GgMatrix mt1;
  GLfloat p1[4];
  GLint x0, y0;

  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // ビューポート上のクリック位置に直す
    x0 = x - vp[0];
    y0 = vp[3] - vp[1] - y;

    // フレームバッファオブジェクト上のクリック位置に直す
    x0 = (GLint)((GLfloat)x0 * (GLfloat)FBOWIDTH / (GLfloat)vp[2] + 0.5f);
    y0 = (GLint)((GLfloat)y0 * (GLfloat)FBOHEIGHT / (GLfloat)vp[3] + 0.5f);

    // クリック位置の奥行きを平行移動
    pick(p1, x0, y0, z0, imc);
    mt1.loadTranslate(p1[0] / p1[3] - p0[0] / p0[3], p1[1] / p1[3] - p0[1] / p0[3], 0.0f);
    mt = mt0 * mt1;
    break;
  case GLUT_RIGHT_BUTTON:
    // トラックボール回転
    tb.motion(x, y);
    break;
  default:
    break;
  }
}

static void keyboard(unsigned char key, int kx, int ky)
{
  switch (key)
  {
    // ESC か q か Q をタイプしたら終了
    case 'Q':
    case 'q':
    case '\033':
      exit(EXIT_SUCCESS);
    default:
      break;
  }
}

static void leave(void)
{
  delete model->getShader();
  delete model;
}

static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
  // シェーダプログラムの読み込み
  pass1 = new GgPass1Shader("pass1.vert", "pass1.frag");

  // 光源
  pass1->setLightPosition(3.0f, 4.0f, 5.0f);
  pass1->setLightAmbient(0.2f, 0.2f, 0.2f);
  pass1->setLightDiffuse(1.0f, 1.0f, 1.0f);
  pass1->setLightSpecular(1.0f, 1.0f, 1.0f);

  // 材質
  pass1->setMaterialAmbient(0.8f, 0.6f, 0.6f);
  pass1->setMaterialDiffuse(0.8f, 0.6f, 0.6f);
  pass1->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  pass1->setMaterialShininess(50.0f);
  
  // OBJ ファイルの読み込み
  model = ggObjArray("model.dat");
  
  // オブジェクトにシェーダを登録
  model->attachShader(pass1);

  // transform feedback buffer に初期値を設定する
  pass1->copyBuffer(model->pnum(), model->pbuf());

  // シェーダプログラムの読み込み
  pass2 = new GgPass2Shader("pass2.vert", "pass2.frag");

  // 画面いっぱいのポリゴンの生成
  rect = ggRectangle(2.0f, 2.0f);
  rect->attachShader(pass2);
  
  // 視野変換行列
  mv.loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // 平行移動
  mt.loadIdentity();
  
  // テクスチャ
  texture0 = new GgTexture(0, FBOWIDTH, FBOHEIGHT, GL_RGBA);
  texture1 = new GgTexture(0, FBOWIDTH, FBOHEIGHT, GL_RGBA32F);

  // レンダーバッファ
  GLuint rb;
  glGenRenderbuffersEXT(1, &rb);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // フレームバッファオブジェクト
  glGenFramebuffersEXT(1, &fb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture0->get(), 0);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, texture1->get(), 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // 初期設定
  glClearColor(0.1f, 0.3f, 0.5f, 0.0f);
  glEnable(GL_CULL_FACE);
  
  // 後始末
  atexit(leave);
}

int main(int argc, char *argv[])
{
  glutInitWindowSize(1024, 1024);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow("Screen Space Motion Blur");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
