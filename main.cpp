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
static GgMatrix mv;   // ����ϊ��s��
static GgMatrix mp;   // ���e�ϊ��s��
static GgMatrix mt;   // ���s�ړ�

/*
** �V�F�[�_
*/
#include "GgPass1Shader.h"
#include "GgPass2Shader.h"
static GgPass1Shader *pass1 = 0;
static GgPass2Shader *pass2 = 0;

/*
** OBJ �t�@�C��
*/
static GgTriangles *model = 0;

/*
** ��ʂ����ς��̋�`
*/
static GgTriangles *rect = 0;

/*
** �e�N�X�`��
*/
static GgTexture *texture0 = 0;
static GgTexture *texture1 = 0;

/*
** �t���[���o�b�t�@�I�u�W�F�N�g
*/
#define FBOWIDTH 1024
#define FBOHEIGHT 1024
static GLuint fb;

/*
** �r���[�|�[�g
*/
static int vp[4];

static void display(void)
{
  // �����_�[�^�[�Q�b�g�̃��X�g
  static const GLenum bufs[] =
  {
    GL_COLOR_ATTACHMENT0_EXT, //   �F
    GL_COLOR_ATTACHMENT1_EXT, //   ���x
  };
  
  // �t���[���o�b�t�@�I�u�W�F�N�g�w��
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  
  // �����_�[�^�[�Q�b�g�w��
  glDrawBuffers(sizeof bufs / sizeof bufs[0], bufs);
  
  // �r���[�|�[�g�̐ݒ�
  glViewport(0, 0, FBOWIDTH, FBOHEIGHT);
  
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �����_�����O
  glEnable(GL_DEPTH_TEST);
  pass1->loadMatrix(mp, mv * mt * tb.get());
  model->draw();
  pass1->swapBuffers();
  
  // �t���[���o�b�t�@�I�u�W�F�N�g����
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // �����_�[�^�[�Q�b�g���A
  glDrawBuffer(GL_BACK);
  
  // �J���[�e�N�X�`���̎g�p
  texture0->use(0);
  texture1->use(1);

  // ��ʂ����ς��̋�`�̕`��
  glViewport(vp[0], vp[1], vp[2], vp[3]);
  glDisable(GL_DEPTH_TEST);
  rect->draw();

  // �J���[�e�N�X�`���̉��
  texture0->unuse();
  texture1->unuse();

  // �_�u���o�b�t�@�����O
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  // �E�B���h�E�S�̂ɕ\��
  glViewport(vp[0] = 0, vp[1] = 0, vp[2] = w, vp[3] = h);
  
  // ���e�ϊ��s��
  mp.loadPerspective(0.6f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);

  // �g���b�N�{�[������͈�
  tb.region(w, h);
  
  // Pass 2 �V�F�[�_�̗����e�[�u���̐���
  pass2->size(1.0f / (GLfloat)w, 1.0f / (GLfloat)h);
}

static void idle(void)
{
  // ��ʂ̕`���ւ�
  glutPostRedisplay();
}

static void pick(GLfloat *pw, GLint x, GLint y, GLfloat z, const GgMatrix &mc)
{
  // �N���b�s���O��Ԓ��̍��W�l
  GLfloat pc[] =
  {
    (GLfloat)x * 2.0f / (GLfloat)FBOWIDTH -  1.0f,
    (GLfloat)y * 2.0f / (GLfloat)FBOHEIGHT - 1.0f,
    z * 2.0f - 1.0f,
    1.0f,
  };

  // ���[���h��Ԓ��̍��W�l
  mc.projection(pw, pc);
}

static GLfloat p0[4], z0;
static GgMatrix mt0, imc;

// ������Ă���{�^��
static int press = -1;

static void mouse(int button, int state, int x, int y)
{
  press = button;
  
  if (state == GLUT_DOWN)
  {
    // �r���[�|�[�g��̃N���b�N�ʒu�ɒ���
    GLint x0 = x - vp[0];
    GLint y0 = vp[3] - vp[1] - y;

    // �t���[���o�b�t�@�I�u�W�F�N�g��̃N���b�N�ʒu�ɒ���
    x0 = (GLint)((GLfloat)x0 * (GLfloat)FBOWIDTH / (GLfloat)vp[2] + 0.5f);
    y0 = (GLint)((GLfloat)y0 * (GLfloat)FBOHEIGHT / (GLfloat)vp[3] + 0.5f);

    // �N���b�N�����Ƃ���̐[�x�l��ǂ�
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    glReadPixels(x0, y0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // �w�i�łȂ��Ƃ�
    if (z0 < 1.0f)
    {
      switch (press)
      {
      case GLUT_LEFT_BUTTON:
        // ���s�ړ��J�n
        imc.loadInvert(mp * mv);
        pick(p0, x0, y0, z0, imc);
        mt0 = mt;
        break;
      case GLUT_RIGHT_BUTTON:
        // �g���b�N�{�[���J�n
        tb.start(x, y);
        break;
      default:
        break;
      }
      
      // �A�j���[�V�����J�n
      glutIdleFunc(idle);
    }
    else
    {
      // �N���b�N���Ȃ��������Ƃɂ���
      press = -1;
    }
  }
  else
  {
    switch (press)
    {
    case GLUT_LEFT_BUTTON:
      // ���s�ړ��I��
      break;
    case GLUT_RIGHT_BUTTON:
      // �g���b�N�{�[���I��
      tb.stop(x, y);
      break;
    default:
      break;
    }
    
    // �A�j���[�V������~
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
    // �r���[�|�[�g��̃N���b�N�ʒu�ɒ���
    x0 = x - vp[0];
    y0 = vp[3] - vp[1] - y;

    // �t���[���o�b�t�@�I�u�W�F�N�g��̃N���b�N�ʒu�ɒ���
    x0 = (GLint)((GLfloat)x0 * (GLfloat)FBOWIDTH / (GLfloat)vp[2] + 0.5f);
    y0 = (GLint)((GLfloat)y0 * (GLfloat)FBOHEIGHT / (GLfloat)vp[3] + 0.5f);

    // �N���b�N�ʒu�̉��s���𕽍s�ړ�
    pick(p1, x0, y0, z0, imc);
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
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  pass1 = new GgPass1Shader("pass1.vert", "pass1.frag");

  // ����
  pass1->setLightPosition(3.0f, 4.0f, 5.0f);
  pass1->setLightAmbient(0.2f, 0.2f, 0.2f);
  pass1->setLightDiffuse(1.0f, 1.0f, 1.0f);
  pass1->setLightSpecular(1.0f, 1.0f, 1.0f);

  // �ގ�
  pass1->setMaterialAmbient(0.8f, 0.6f, 0.6f);
  pass1->setMaterialDiffuse(0.8f, 0.6f, 0.6f);
  pass1->setMaterialSpecular(0.2f, 0.2f, 0.2f);
  pass1->setMaterialShininess(50.0f);
  
  // OBJ �t�@�C���̓ǂݍ���
  model = ggObjArray("model.dat");
  
  // �I�u�W�F�N�g�ɃV�F�[�_��o�^
  model->attachShader(pass1);

  // transform feedback buffer �ɏ����l��ݒ肷��
  pass1->copyBuffer(model->pnum(), model->pbuf());

  // �V�F�[�_�v���O�����̓ǂݍ���
  pass2 = new GgPass2Shader("pass2.vert", "pass2.frag");

  // ��ʂ����ς��̃|���S���̐���
  rect = ggRectangle(2.0f, 2.0f);
  rect->attachShader(pass2);
  
  // ����ϊ��s��
  mv.loadLookat(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // ���s�ړ�
  mt.loadIdentity();
  
  // �e�N�X�`��
  texture0 = new GgTexture(0, FBOWIDTH, FBOHEIGHT, GL_RGBA);
  texture1 = new GgTexture(0, FBOWIDTH, FBOHEIGHT, GL_RGBA32F);

  // �����_�[�o�b�t�@
  GLuint rb;
  glGenRenderbuffersEXT(1, &rb);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // �t���[���o�b�t�@�I�u�W�F�N�g
  glGenFramebuffersEXT(1, &fb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture0->get(), 0);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, texture1->get(), 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  
  // �����ݒ�
  glClearColor(0.1f, 0.3f, 0.5f, 0.0f);
  glEnable(GL_CULL_FACE);
  
  // ��n��
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
