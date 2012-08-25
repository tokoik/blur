#include <cmath>
#include <cstdlib>
#include <cstring>

#include "gg.h"
using namespace gg;

/*
** �g���b�N�{�[������
*/
static GgTrackball tb;

/*
** �ϊ��s��
*/
static GgMatrix mv, imv;   // ����ϊ��s��
static GgMatrix mp, imp;   // ���e�ϊ��s��
static GgMatrix mt;        // ���s�ړ�

/*
** OBJ �t�@�C��
*/
static GgObject *model = 0;

/*
** �V�F�[�_
*/
#include "GgSimpleShader.h"

/*
** �E�B���h�E�̍���
*/
static int height;

static void display(void)
{
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �}�`�̕`��
  model->getShader()->loadMatrix(mp, mv * mt * tb.get());
  model->draw();
  
  // �_�u���o�b�t�@�����O
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // �E�B���h�E�S�̂ɕ\��
  glViewport(0, 0, w, height = h);
  
  // ���e�ϊ��s��
  mp.loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);
  imp.loadInvert(mp);

  // �g���b�N�{�[������͈�
  tb.region(w, h);
}

static void idle(void)
{
  // ��ʂ̕`���ւ�
  glutPostRedisplay();
}

static void pick(GLfloat *pw, int x, int y, GLfloat z, const GgMatrix &m)
{
  // �r���[�|�[�g�̎��o��
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  
  // �N���b�s���O��Ԓ��̍��W�l
  GLfloat pc[] =
  {
    (GLfloat)(x - vp[0]) * 2.0f / (GLfloat)vp[2] - 1.0f,
    (GLfloat)(y - vp[1]) * 2.0f / (GLfloat)vp[3] - 1.0f,
    z * 2.0f - 1.0f,
    1.0f,
  };
  
  // ���[���h��Ԓ��̍��W�l
  m.projection(pw, pc);
}

static GLfloat p0[4], z0;
static GgMatrix mt0;

// ������Ă���{�^��
static int press = -1;

static void mouse(int button, int state, int x, int y)
{
  switch (press = button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z0);
      if (z0 < 1.0f)
      {
        // ���s�ړ��J�n
        pick(p0, x, height - y, z0, imv * imp);
        mt0 = mt;
        glutIdleFunc(idle);
      }
      else {
        press = -1;
      }
    }
    else {
      // ���s�ړ���~
      glutIdleFunc(0);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
    {
      // �g���b�N�{�[���J�n
      tb.start(x, y);
      glutIdleFunc(idle);
    }
    else {
      // �g���b�N�{�[����~
      tb.stop(x, y);
      glutIdleFunc(0);
    }
    break;
  default:
    break;
  }
}

static void motion(int x, int y)
{
  GgMatrix mt1;
  GLfloat p1[4];

  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // �N���b�N�ʒu�̉��s���𕽍s�ړ�
    pick(p1, x, height - y, z0, imv * imp);
    mt1.loadTranslate(p1[0] / p1[3] - p0[0] / p0[3], p1[1] / p1[3] - p0[1] / p0[3], 0.0f);
    mt = mt0 * mt1;
    break;
  case GLUT_RIGHT_BUTTON:
    // �g���b�N�{�[����]
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
  delete model->getShader();
  delete model;
}

static void init(void)
{
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  GgSimpleShader *simple = new GgSimpleShader("simple.vert", "simple.frag");

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
  
  // ����ϊ��s��
  mv.loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  imv.loadInvert(mv);
  
  // ���s�ړ�
  mt.loadIdentity();
  
  // �����ݒ�
  glClearColor(0.0f, 0.2f, 0.4f, 0.0f);
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
