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
** OBJ ファイル
*/
static GgObject *model = 0;

/*
** シェーダ
*/
#include "GgBlurShader.h"

/*
** ウィンドウの高さ
*/
static int height;

static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // 図形の描画
  GgBlurShader *blur = dynamic_cast<GgBlurShader *>(model->getShader());
  if (blur != 0)
  {
    blur->loadMatrix(mp, mv * mt * tb.get());
    model->draw();
    ggError("draw");
    blur->swapBuffers();
  }
  
  // ダブルバッファリング
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // ウィンドウ全体に表示
  glViewport(0, 0, w, height = h);
  
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
    glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z0);
    
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
      exit(0);
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
  GgBlurShader *blur = new GgBlurShader("blur.vert", "blur.frag");

  // 光源
  blur->setLightPosition(3.0f, 4.0f, 5.0f);
  blur->setLightAmbient(0.2f, 0.2f, 0.2f);
  blur->setLightDiffuse(1.0f, 1.0f, 1.0f);
  blur->setLightSpecular(1.0f, 1.0f, 1.0f);

  // 材質
  blur->setMaterialAmbient(0.8f, 0.6f, 0.6f);
  blur->setMaterialDiffuse(0.8f, 0.6f, 0.6f);
  blur->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  blur->setMaterialShininess(50.0f);
  
  // OBJ ファイルの読み込み
  model = ggObj("model.dat");
  
  // オブジェクトにシェーダを登録
  model->attachShader(blur);

  // transform feedback buffer に初期値を設定する
  blur->copyBuffer(model->pnum(), model->pbuf());
  
  // 視野変換行列
  mv.loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  imv.loadInvert(mv);
  
  // 平行移動
  mt.loadIdentity();
  
  // 初期設定
  glClearColor(0.0f, 0.2f, 0.4f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  // 後始末
  atexit(leave);
}

int main(int argc, char *argv[])
{
  glutInitWindowSize(1024, 1024);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
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
