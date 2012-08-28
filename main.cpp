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
static GgMatrix mv, imv;   // 視野変換行列
static GgMatrix mp, imp;   // 投影変換行列
static GgMatrix mt;        // 平行移動

/*
** シェーダ
*/
#include "GgPass1Shader.h"
#include "GgPass2Shader.h"

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
static GgTexture *texture0, *texture1;

/*
** フレームバッファオブジェクト
*/
#define FBOWIDTH 1024
#define FBOHEIGHT 1024
static GLuint fb;           // フレームバッファオブジェクト

/*
** ウィンドウの幅と高さ
*/
static int width, height;

static void display(void)
{
  // 図形の描画
  GgPass1Shader *pass1 = dynamic_cast<GgPass1Shader *>(model->getShader());
  if (pass1 != 0)
  {
    // レンダーターゲットのリスト
    static const GLenum bufs[] =
    {
      GL_COLOR_ATTACHMENT0_EXT, //   色
      GL_COLOR_ATTACHMENT1_EXT, //   速度
    };

    // フレームバッファオブジェクト指定
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    ggError("glBindFramebufferEXT");

    // レンダーターゲット指定
    glDrawBuffers(sizeof bufs / sizeof bufs[0], bufs);
    ggError("glDrawBuffers");

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
  }
  
  // カラーテクスチャの使用
  texture0->use(0);
  texture1->use(1);

  // 画面いっぱいの矩形の描画
  glViewport(0, 0, width, height);
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
  glViewport(0, 0, width = w, height = h);
  
  // 投影変換行列
  mp.loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);
  imp.loadInvert(mp);

  // トラックボールする範囲
  tb.region(w, h);
}

static void idle(void)
{
  // 画面の描き替え
  glutPostRedisplay();
}

static void pick(GLfloat *pw, int x, int y, GLfloat z, const GgMatrix &m)
{
  // ビューポートの取り出し
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  
  // クリッピング空間中の座標値
  GLfloat pc[] =
  {
    (GLfloat)(x - vp[0]) * 2.0f / (GLfloat)vp[2] - 1.0f,
    (GLfloat)(y - vp[1]) * 2.0f / (GLfloat)vp[3] - 1.0f,
    z * 2.0f - 1.0f,
    1.0f,
  };
  
  // ワールド空間中の座標値
  m.projection(pw, pc);
}

static GLfloat p0[4], z0;
static GgMatrix mt0;

// 押されているボタン
static int press = -1;

static void mouse(int button, int state, int x, int y)
{
  press = button;
  
  if (state == GLUT_DOWN)
  {
    // クリックしたところの深度値を読む
    ggError("++glBindFramebufferEXT");
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    ggError("--glBindFramebufferEXT");
    glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z0);
    ggError("glReadPixels");
    
    // 背景でないとき
    if (z0 < 1.0f)
    {
      switch (press)
      {
      case GLUT_LEFT_BUTTON:
        // 平行移動開始
        pick(p0, x, height - y, z0, imv * imp);
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
      press = -1;
    }
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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

  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // クリック位置の奥行きを平行移動
    pick(p1, x, height - y, z0, imv * imp);
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
  GgPass1Shader *pass1 = new GgPass1Shader("pass1.vert", "pass1.frag");

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
  GgPointShader *pass2 = new GgPass2Shader("pass2.vert", "pass2.frag");

  // 画面いっぱいのポリゴンの生成
  rect = ggRectangle(2.0f, 2.0f);
  rect->attachShader(pass2);
  
  // 視野変換行列
  mv.loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  imv.loadInvert(mv);
  
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
