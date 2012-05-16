#include <cstdlib>
#include <cstring>
#include <cmath>

#include "gg.h"
using namespace gg;

/*
** �g���b�N�{�[������
*/
#include "Trackball.h"
static Trackball *tb = 0;

/*
** ���e�ϊ��s��
*/
#include "Matrix.h"
static Matrix *mp = 0;

/*
** �n��
*/
#include "Square.h"
static Square *ground = 0;

/*
** OBJ �t�@�C��
*/
#include "Obj.h"
static Obj *walker = 0;

/*
** ����
*/
static const GLfloat lpos[] = { 3.0f, 4.0f, 5.0f, 1.0 };
static const GLfloat lamb[] = { 0.2f, 0.2f, 0.2f, 1.0 };
static const GLfloat ldiff[] = { 1.0f, 1.0f, 1.0f, 1.0 };
static const GLfloat lspec[] = { 1.0f, 1.0f, 1.0f, 1.0 };

/*
** �V�F�[�_
*/
#include "SimpleShader.h"
static SimpleShader *simple = 0;
#include "TileShader.h"
static TileShader *tile = 0;

/*
** �t���[���o�b�t�@�I�u�W�F�N�g
*/
#define FBOWIDTH  1024      // �t���[���o�b�t�@�I�u�W�F�N�g�̕�
#define FBOHEIGHT 1024      // �t���[���o�b�t�@�I�u�W�F�N�g�̍���
static GLuint fb;           // �f�荞�ݗp�̃t���[���o�b�t�@�I�u�W�F�N�g
static GLuint cb;           // �J���[�o�b�t�@�p�̃e�N�X�`��
static GLuint rb;           // �f�v�X�o�b�t�@�p�̃����_�[�o�b�t�@
static GLuint sb;           // �V���h�E�}�b�v�p�̃t���[���o�b�t�@�I�u�W�F�N�g
static GLuint db;           // �V���h�E�}�b�v�p�̃e�N�X�`��

static void display(void)
{
  // �r���[�|�[�g�̕ۑ��Ɛݒ�
  tile->saveViewport();
  glViewport(0, 0, FBOWIDTH, FBOHEIGHT);
  
  // �V�[���S�̂����߂铊�e�ϊ��s��
  Matrix mq;
  mq.loadPerspective(0.2f, 1.0f, 6.0f, 10.0f);
  
  // ���������_�ɂ�������ϊ��s��
  Matrix ml;
  ml.loadLookat(lpos[0], lpos[1], lpos[2], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  
  // �t���[���o�b�t�@�I�u�W�F�N�g����������
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sb);
  
  // �f�v�X�o�b�t�@���N���A
  glClear(GL_DEPTH_BUFFER_BIT);
  
  // �V���h�E�}�b�v�̍쐬
  simple->setLightPosition(lpos);
  simple->loadMatrix(mq, ml * tb->get());
  walker->setCullFace(false);
  walker->draw();
  
  // �t���[���o�b�t�@�I�u�W�F�N�g�̌�������������
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // ����ϊ��s��
  Matrix mv;
  mv.loadLookat(0.0f, 1.5f, 3.0f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // �����ϊ�
  Matrix mr;
  mr.loadTranslate(0.0f, -0.6f, 0.0f);
  mr.scale(1.0f, -1.0f, 1.0f);
  mr.translate(0.0f, 0.6f, 0.0f);
  
  // �����̌����ʒu
  GLfloat rlpos[4];
  mr.projection(rlpos, lpos);
  rlpos[0] = -rlpos[0];
  rlpos[1] = -rlpos[1];
  rlpos[2] = -rlpos[2];
  
  // �t���[���o�b�t�@�I�u�W�F�N�g����������
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  // �J���[�o�b�t�@�ƃf�v�X�o�b�t�@���N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �����̕`��
  simple->setLightPosition(rlpos);
  simple->loadMatrix(*mp, mv * mr * tb->get());
  walker->setCullFace(false);
  walker->draw();
  
  // �t���[���o�b�t�@�I�u�W�F�N�g�̌�������������
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // �r���[�|�[�g�̕��A
  tile->restoreViewport();
  
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �}�`�̕`��
  simple->setLightPosition(lpos);
  simple->loadMatrix(*mp, mv * tb->get());
  walker->setCullFace(true);
  walker->draw();
  
  // ���ʂ̕`��
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

  // �_�u���o�b�t�@�����O
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // �E�B���h�E�S�̂ɕ\��
  glViewport(0, 0, w, h);
  
  // ���e�ϊ��s��
  mp->loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);

  // �g���b�N�{�[������͈�
  tb->region(w, h);
}

static void idle(void)
{
  // ��ʂ̕`���ւ�
  glutPostRedisplay();
}

// ������Ă���{�^��
static int press = 0;

static void mouse(int button, int state, int x, int y)
{
  switch (press = button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      // �g���b�N�{�[���J�n
      tb->start(x, y);
      glutIdleFunc(idle);
    }
    else {
      // �g���b�N�{�[����~
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
    // �g���b�N�{�[���ړ�
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
    // ESC �� q �� Q ���^�C�v������I��
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
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  simple = new SimpleShader("simple.vert", "simple.frag");
  tile = new TileShader("tile.vert", "tile.frag");
  
  // ����
  simple->setLightAmbient(lamb);
  simple->setLightDiffuse(ldiff);
  simple->setLightSpecular(lspec);
  tile->setLightPosition(lpos);
  tile->setLightAmbient(lamb);
  tile->setLightDiffuse(ldiff);
  tile->setLightSpecular(lspec);
  
  // �ގ�
  simple->setMaterialAmbient(0.4f, 0.5f, 0.0f);
  simple->setMaterialDiffuse(0.4f, 0.5f, 0.0f);
  simple->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  simple->setMaterialShininess(50.0f);
  tile->setMaterialAmbient(0.8f, 0.8f, 0.8f);
  tile->setMaterialDiffuse(0.8f, 0.8f, 0.8f);
  tile->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  tile->setMaterialShininess(30.0f);
  
  // OBJ �t�@�C���̓ǂݍ���
  walker = new Obj("scoutwalker.obj");
  walker->setShader(simple);
  
  // �n��
  ground = new Square(3.0f, 3.0f);
  ground->setShader(tile);
  
  // �ϊ��s��
  mp = new Matrix;
  
  // �g���b�N�{�[������
  tb = new Trackball;
  
  // �J���[�o�b�t�@�p�̃e�N�X�`����p�ӂ���
  glGenTextures(1, &cb);
  glBindTexture(GL_TEXTURE_2D, cb);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FBOWIDTH, FBOHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // �f�v�X�o�b�t�@�p�̃����_�[�o�b�t�@��p�ӂ���
  glGenRenderbuffersEXT(1, &rb);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // �f�荞�ݗp�̃t���[���o�b�t�@�I�u�W�F�N�g���쐬����
  glGenFramebuffersEXT(1, &fb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  // �f�v�X�o�b�t�@�p�̃e�N�X�`����p�ӂ���
  glGenTextures(1, &db);
  glBindTexture(GL_TEXTURE_2D, db);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // �������ރ|���S���̃e�N�X�`�����W�l�̂q�ƃe�N�X�`���Ƃ̔�r���s���悤�ɂ���
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  // �����q�̒l���e�N�X�`���̒l�ȉ��Ȃ�^�i�܂�����j
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  // ��r�̌��ʂ��P�x�l�Ƃ��ē���
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // �V���h�E�}�b�s���O�p�̃t���[���o�b�t�@�I�u�W�F�N�g���쐬����
  glGenFramebuffersEXT(1, &sb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, db, 0);
  // �J���[�o�b�t�@�͖����̂ŕ`���Ȃ�
  glDrawBuffer(GL_NONE);
  // �J���[�o�b�t�@�͖����̂œǂ܂Ȃ�
  glReadBuffer(GL_NONE);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // �����ݒ�
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  // ��n��
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
