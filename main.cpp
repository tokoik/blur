#include <cstdlib>
#include <cstring>
#include <cmath>

#include "gg.h"
using namespace gg;

/*
** トラックボール処理
*/
#include "Trackball.h"
static Trackball *tb = 0;

/*
** 投影変換行列
*/
#include "Matrix.h"
static Matrix *mp = 0;

/*
** 地面
*/
#include "Square.h"
static Square *ground = 0;

/*
** OBJ ファイル
*/
#include "Obj.h"
static Obj *walker = 0;

/*
** 光源
*/
static const GLfloat lpos[] = { 3.0f, 4.0f, 5.0f, 1.0 };
static const GLfloat lamb[] = { 0.2f, 0.2f, 0.2f, 1.0 };
static const GLfloat ldiff[] = { 1.0f, 1.0f, 1.0f, 1.0 };
static const GLfloat lspec[] = { 1.0f, 1.0f, 1.0f, 1.0 };

/*
** シェーダ
*/
#include "SimpleShader.h"
static SimpleShader *simple = 0;
#include "TileShader.h"
static TileShader *tile = 0;

/*
** フレームバッファオブジェクト
*/
#define FBOWIDTH  1024      // フレームバッファオブジェクトの幅
#define FBOHEIGHT 1024      // フレームバッファオブジェクトの高さ
static GLuint fb;           // 映り込み用のフレームバッファオブジェクト
static GLuint cb;           // カラーバッファ用のテクスチャ
static GLuint rb;           // デプスバッファ用のレンダーバッファ
static GLuint sb;           // シャドウマップ用のフレームバッファオブジェクト
static GLuint db;           // シャドウマップ用のテクスチャ

static void display(void)
{
  // ビューポートの保存と設定
  tile->saveViewport();
  glViewport(0, 0, FBOWIDTH, FBOHEIGHT);
  
  // シーン全体を収める投影変換行列
  Matrix mq;
  mq.loadPerspective(0.2f, 1.0f, 6.0f, 10.0f);
  
  // 光源を視点にした視野変換行列
  Matrix ml;
  ml.loadLookat(lpos[0], lpos[1], lpos[2], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  
  // フレームバッファオブジェクトを結合する
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sb);
  
  // デプスバッファをクリア
  glClear(GL_DEPTH_BUFFER_BIT);
  
  // シャドウマップの作成
  simple->setLightPosition(lpos);
  simple->loadMatrix(mq, ml * tb->get());
  walker->setCullFace(false);
  walker->draw();
  
  // フレームバッファオブジェクトの結合を解除する
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // 視野変換行列
  Matrix mv;
  mv.loadLookat(0.0f, 1.5f, 3.0f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // 鏡像変換
  Matrix mr;
  mr.loadTranslate(0.0f, -0.6f, 0.0f);
  mr.scale(1.0f, -1.0f, 1.0f);
  mr.translate(0.0f, 0.6f, 0.0f);
  
  // 鏡像の光源位置
  GLfloat rlpos[4];
  mr.projection(rlpos, lpos);
  rlpos[0] = -rlpos[0];
  rlpos[1] = -rlpos[1];
  rlpos[2] = -rlpos[2];
  
  // フレームバッファオブジェクトを結合する
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  // カラーバッファとデプスバッファをクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // 鏡像の描画
  simple->setLightPosition(rlpos);
  simple->loadMatrix(*mp, mv * mr * tb->get());
  walker->setCullFace(false);
  walker->draw();
  
  // フレームバッファオブジェクトの結合を解除する
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // ビューポートの復帰
  tile->restoreViewport();
  
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // 図形の描画
  simple->setLightPosition(lpos);
  simple->loadMatrix(*mp, mv * tb->get());
  walker->setCullFace(true);
  walker->draw();
  
  // 床面の描画
  Matrix mm;
  mm.loadTranslate(0.0f, -0.6f, 0.0f);
  mm.rotate(1.0f, 0.0f, 0.0f, -1.5707963f);
  tile->loadMatrix(*mp, mv * mm);
  Matrix ms;
  ms.loadTranslate(0.5, 0.5, 0.5);
  ms.scale(0.5, 0.5, 0.5);
  tile->loadShadowMatrix(ms * mq * ml * mm);
  tile->setTexture0(0, cb);
  tile->setTexture1(1, db);
  ground->draw();

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
  switch (press = button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
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
  switch (press) {
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
  switch (key) {
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
  delete walker;
  delete ground;
  delete mp;
  delete tb;
}

static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();
  
  // シェーダプログラムの読み込み
  simple = new SimpleShader("simple.vert", "simple.frag");
  tile = new TileShader("tile.vert", "tile.frag");
  
  // 光源
  simple->setLightAmbient(lamb);
  simple->setLightDiffuse(ldiff);
  simple->setLightSpecular(lspec);
  tile->setLightPosition(lpos);
  tile->setLightAmbient(lamb);
  tile->setLightDiffuse(ldiff);
  tile->setLightSpecular(lspec);
  
  // 材質
  simple->setMaterialAmbient(0.4f, 0.5f, 0.0f);
  simple->setMaterialDiffuse(0.4f, 0.5f, 0.0f);
  simple->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  simple->setMaterialShininess(50.0f);
  tile->setMaterialAmbient(0.8f, 0.8f, 0.8f);
  tile->setMaterialDiffuse(0.8f, 0.8f, 0.8f);
  tile->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  tile->setMaterialShininess(30.0f);
  
  // OBJ ファイルの読み込み
  walker = new Obj("scoutwalker.obj");
  walker->setShader(simple);
  
  // 地面
  ground = new Square(3.0f, 3.0f);
  ground->setShader(tile);
  
  // 変換行列
  mp = new Matrix;
  
  // トラックボール処理
  tb = new Trackball;
  
  // カラーバッファ用のテクスチャを用意する
  glGenTextures(1, &cb);
  glBindTexture(GL_TEXTURE_2D, cb);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTH, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // デプスバッファ用のレンダーバッファを用意する
  glGenRenderbuffersEXT(1, &rb);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // 映り込み用のフレームバッファオブジェクトを作成する
  glGenFramebuffersEXT(1, &fb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  // デプスバッファ用のテクスチャを用意する
  glGenTextures(1, &db);
  glBindTexture(GL_TEXTURE_2D, db);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  // もしＲの値がテクスチャの値以下なら真（つまり日向）
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  // 比較の結果を輝度値として得る
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // シャドウマッピング用のフレームバッファオブジェクトを作成する
  glGenFramebuffersEXT(1, &sb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, db, 0);
  // カラーバッファは無いので描かない
  glDrawBuffer(GL_NONE);
  // カラーバッファは無いので読まない
  glReadBuffer(GL_NONE);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // 初期設定
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  // 後始末
  atexit(leave);
}

int main(int argc, char *argv[])
{
  glutInitWindowSize(800, 800);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("Walker");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
