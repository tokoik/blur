#include <cmath>
#include <cstdlib>
#include <cstring>

#include "gg.h"
using namespace gg;

/*
** トラックボール処理
*/
static GgTrackball *tb = 0;

/*
** 変換行列
*/
static GgMatrix *mv = 0;  // 視野変換行列
static GgMatrix *mp = 0;  // 投影変換行列

/*
** OBJ ファイル
*/
static GgObject *model = 0;

/*
** 点データ
*/
static GgPoints *points = 0;

/*
** シェーダ
*/
#include "GgSimpleShader.h"

static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // 図形の描画
  GgSimpleShader *simple = dynamic_cast<GgSimpleShader *>(model->getShader());
  if (simple) simple->loadMatrix(*mp, *mv);
  model->draw();
  
  // 点の描画
  GgPointShader *sphere = dynamic_cast<GgPointShader *>(points->getShader());
  if (sphere) sphere->loadModelViewProjectionMatrix(*mp * *mv * tb->get());
  points->draw();
  
  // ダブルバッファリング
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // ウィンドウ全体に表示
  glViewport(0, 0, w, h);
  
  // 投影変換行列
  mp->loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);

  // トラックボールする範囲
  tb->region(w, h);
}

static void idle(void)
{
  // 画面の描き替え
  glutPostRedisplay();
}

// 押されているボタン
static int press = 0;

static void mouse(int button, int state, int x, int y)
{
  switch (press = button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      // トラックボール開始
      tb->start(x, y);
      glutIdleFunc(idle);
    }
    else {
      // トラックボール停止
      tb->stop(x, y);
      glutIdleFunc(0);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
}

static void motion(int x, int y)
{
  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // トラックボール移動
    tb->motion(x, y);
    break;
  case GLUT_RIGHT_BUTTON:
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
  delete model;
  delete points;
  delete mv;
  delete mp;
  delete tb;

  // オブジェクトにアタッチしたシェーダは
  // オブジェクトの削除時に削除されるので
  // ここでは delete しない
}

static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
  // シェーダプログラムの読み込み
  GgSimpleShader *simple = new GgSimpleShader("simple.vert", "simple.frag");
  GgPointShader *sphere = new GgPointShader("sphere.vert", "sphere.frag", "sphere.geom", GL_POINTS, GL_TRIANGLE_STRIP, 3);

  // 光源
  simple->setLightPosition(3.0f, 4.0f, 5.0f);
  simple->setLightAmbient(0.2f, 0.2f, 0.2f);
  simple->setLightDiffuse(1.0f, 1.0f, 1.0f);
  simple->setLightSpecular(1.0f, 1.0f, 1.0f);

  // 材質
  simple->setMaterialAmbient(0.8f, 0.6f, 0.6f);
  simple->setMaterialDiffuse(0.8f, 0.6f, 0.6f);
  simple->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  simple->setMaterialShininess(50.0f);
  
  // OBJ ファイルの読み込み
  model = ggObj("model.dat");
  model->attachShader(simple);
  
  // 点
  points = ggPointSphere(100, 0.0f, 0.0f, 0.0f, 1.0f);
  points->attachShader(sphere);
  
  // 視野変換行列
  mv = new GgMatrix;
  mv->loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // 投影変換行列
  mp = new GgMatrix;
  
  // トラックボール処理
  tb = new GgTrackball;

  // 初期設定
  glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  // 後始末
  atexit(leave);
}

int main(int argc, char *argv[])
{
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
