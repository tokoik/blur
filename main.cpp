#include <cmath>
#include <cstdlib>
#include <cstring>

#include "gg.h"
using namespace gg;

/*
** �g���b�N�{�[������
*/
static GgTrackball *tb = 0;

/*
** �ϊ��s��
*/
static GgMatrix *mv = 0;  // ����ϊ��s��
static GgMatrix *mp = 0;  // ���e�ϊ��s��

/*
** OBJ �t�@�C��
*/
static GgObject *model = 0;

/*
** �_�f�[�^
*/
static GgPoints *points = 0;

/*
** �V�F�[�_
*/
#include "GgSimpleShader.h"

static void display(void)
{
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �}�`�̕`��
  GgSimpleShader *simple = dynamic_cast<GgSimpleShader *>(model->getShader());
  if (simple) simple->loadMatrix(*mp, *mv);
  model->draw();
  
  // �_�̕`��
  GgPointShader *sphere = dynamic_cast<GgPointShader *>(points->getShader());
  if (sphere) sphere->loadModelViewProjectionMatrix(*mp * *mv * tb->get());
  points->draw();
  
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
  switch (press = button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
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
  switch (press)
  {
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
  switch (key)
  {
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
  delete model;
  delete points;
  delete mv;
  delete mp;
  delete tb;

  // �I�u�W�F�N�g�ɃA�^�b�`�����V�F�[�_��
  // �I�u�W�F�N�g�̍폜���ɍ폜�����̂�
  // �����ł� delete ���Ȃ�
}

static void init(void)
{
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  GgSimpleShader *simple = new GgSimpleShader("simple.vert", "simple.frag");
  GgPointShader *sphere = new GgPointShader("sphere.vert", "sphere.frag", "sphere.geom", GL_POINTS, GL_TRIANGLE_STRIP, 3);

  // ����
  simple->setLightPosition(3.0f, 4.0f, 5.0f);
  simple->setLightAmbient(0.2f, 0.2f, 0.2f);
  simple->setLightDiffuse(1.0f, 1.0f, 1.0f);
  simple->setLightSpecular(1.0f, 1.0f, 1.0f);

  // �ގ�
  simple->setMaterialAmbient(0.8f, 0.6f, 0.6f);
  simple->setMaterialDiffuse(0.8f, 0.6f, 0.6f);
  simple->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  simple->setMaterialShininess(50.0f);
  
  // OBJ �t�@�C���̓ǂݍ���
  model = ggObj("model.dat");
  model->attachShader(simple);
  
  // �_
  points = ggPointSphere(100, 0.0f, 0.0f, 0.0f, 1.0f);
  points->attachShader(sphere);
  
  // ����ϊ��s��
  mv = new GgMatrix;
  mv->loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // ���e�ϊ��s��
  mp = new GgMatrix;
  
  // �g���b�N�{�[������
  tb = new GgTrackball;

  // �����ݒ�
  glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  // ��n��
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
