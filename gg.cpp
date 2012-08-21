/*
** �Q�[���O���t�B�b�N�X���_�p�⏕�v���O����
**

Copyright (c) 2011, 2012 Kohe Tokoi. All Rights Reserved.

Permission is hereby granted, free of charge,  to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction,  including without limitation the rights 
to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell 
copies or substantial portions of the Software.

The above  copyright notice  and this permission notice  shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR 
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY, 
FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL 
KOHE TOKOI  BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY,  WHETHER IN 
AN ACTION  OF CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM,  OUT OF  OR  IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <float.h>

#include "gg.h"

/*
** �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
*/
void gg::ggInit(void)
{
  // �o�[�W�����`�F�b�N
  if (atof(reinterpret_cast<const char *>(glGetString(GL_VERSION))) < 2.1)
  {
    std::cerr << "Error: This program requires OpenGL 2.1 or lator." << std::endl;
    exit(1);
  }

  // Swap Interval �̐ݒ�
#if defined(X11)
  Display *dpy = glXGetCurrentDisplay();
  GLXDrawable drawable = glXGetCurrentDrawable();
  if (drawable)
  {
    glXSwapIntervalEXT(dpy, drawable, 1);
    return;
  }
#elif defined(__APPLE__)
  int swap_interval = 1;
  CGLContextObj cgl_context = CGLGetCurrentContext();
  if (!CGLSetParameter(cgl_context, kCGLCPSwapInterval, &swap_interval))
    return;
#elif defined(WIN32)
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    std::cerr <<  "Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }
#  if defined(WGLEW_EXT_swap_control)
  if (WGLEW_EXT_swap_control)
  {
    wglSwapIntervalEXT(1);
    return;
  }
#  endif
#else
  std::cerr << "Warning: Could not set swap interval" << std::endl;
#endif
}

/*
** OpenGL �̃G���[�`�F�b�N
*/
void gg::ggError(const char *msg)
{
  GLenum error = glGetError();

  if (error != GL_NO_ERROR)
  {
    if (msg) std::cerr << msg << ": ";

    switch (error)
    {
    case GL_INVALID_ENUM:
      std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
      break;
    case GL_INVALID_VALUE:
      std::cerr << "A numeric argument is out of range" << std::endl;
      break;
    case GL_INVALID_OPERATION:
      std::cerr << "The specified operation is not allowed in the current state" << std::endl;
      break;
    case GL_STACK_OVERFLOW:
      std::cerr << "This command would cause a stack overflow" << std::endl;
      break;
    case GL_STACK_UNDERFLOW:
      std::cerr << "This command would cause a a stack underflow" << std::endl;
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr << "There is not enough memory left to execute the command" << std::endl;
      break;
    case GL_TABLE_TOO_LARGE:
      std::cerr << "The specified table exceeds the implementation's maximum supported table size" << std::endl;
      break;
#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#    define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#endif
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;
      break;
    default:
      std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;
      break;
    }
  }
}

/*
** FBO �̃G���[�`�F�b�N
*/
void gg::ggFBOError(const char *msg)
{
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

  if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    if (msg) std::cerr << msg << ": ";

    switch (status)
    {
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      std::cerr << "Unsupported framebuffer format" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      std::cerr << "Framebuffer incomplete, missing attachment" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      std::cerr << "Framebuffer incomplete, duplicate attachment" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      std::cerr << "Framebuffer incomplete, attached images must have same dimensions" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      std::cerr << "Framebuffer incomplete, attached images must have same format" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      std::cerr << "Framebuffer incomplete, missing draw buffer" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      std::cerr << "Framebuffer incomplete, missing read buffer" << std::endl;
      break;
    default:
      std::cerr << "Programming error; will fail on all hardware: " << std::hex << std::showbase << status << std::endl;
      break;
    }
  }
}

/*
** �V�F�[�_�[�̃\�[�X�v���O�������������ɓǂݍ���
*/
static bool readShaderSource(GLuint shader, const char *name)
{
  bool ret = true; // �I���X�e�[�^�X�itrue �Ȃ�G���[�j
  
  // �\�[�X�t�@�C�����J��
  std::ifstream file(name, std::ios::binary);

  if (file.fail())
  {
    // �J���Ȃ�����
    std::cerr << "Error: Can't open source file: " << name << std::endl;
  }
  else
  {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    
    // �t�@�C���T�C�Y�̃��������m��
    char *buffer = new GLchar[length];
    
    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(buffer, length);

    if (file.bad())
    {
      // ���܂��ǂݍ��߂Ȃ�����
      std::cerr << "Error: Could not read souce file: " << name << std::endl;
    }
    else {
      // �V�F�[�_�̃\�[�X�v���O�����̃V�F�[�_�I�u�W�F�N�g�ւ̓ǂݍ���
      const GLchar *source[] = { buffer };
      glShaderSource(shader, 1, source, &length);
      ret = false;
    }
    file.close();
    
    // �ǂݍ��݂Ɏg�������������J������
    delete[] buffer;
  }
  
  return ret;
}

/*
** �V�F�[�_�̏���\������
*/
static void printShaderInfoLog(GLuint shader)
{
  // �V�F�[�_�̃R���p�C�����̃��O�̒������擾����
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // �V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** �v���O�����̏���\������
*/
static void printProgramInfoLog(GLuint program)
{
  // �V�F�[�_�̃����N���̃��O�̒������擾����
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1)
  {
    // �V�F�[�_�̃����N���̃��O�̓��e���擾����
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** �V�F�[�_�[�\�[�X�t�@�C���̓ǂݍ���
*/
GLuint gg::loadShader(
  const char *vert,       // �o�[�e�b�N�X�V�F�[�_�̃\�[�X�t�@�C����
  const char *frag,       // �t���O�����g�V�F�[�_�̃\�[�X�t�@�C����
  const char *geom,       // �W�I���g���V�F�[�_�̃\�[�X�t�@�C����
  GLenum input,           // �W�I���g���V�F�[�_�̓��̓v���~�e�B�u
  GLenum output,          // �W�I���g���V�F�[�_�̏o�̓v���~�e�B�u
  int vertices,           // �W�I���g���V�F�[�_�̏o�͒��_��
  int nvarying,           // Transform Feedback ���� varying �ϐ��̐�
  const char **varyings   // Transform Feedback ���� varying �ϐ��̃��X�g
  )
{
  // �V�F�[�_�v���O�����̍쐬
  GLuint program = glCreateProgram();

  if (program > 0)
  {
    GLint compiled, linked; // �R���p�C������, �����N����

    // �o�[�e�b�N�X�V�F�[�_�̍쐬
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    
    // �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̓ǂݍ���
    if (readShaderSource(vertShader, vert))
    {
      glDeleteShader(vertShader);
      glDeleteProgram(program);
      return 0;
    }
    
    // �o�[�e�b�N�X�V�F�[�_�̃R���p�C��
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    printShaderInfoLog(vertShader);
    if (compiled == GL_FALSE)
    {
      std::cerr << "Error: Could not compile vertex shader source: " << vert << std::endl;
      glDeleteShader(vertShader);
      glDeleteProgram(program);
      return 0;
    }
    
    // �o�[�e�b�N�X�V�F�[�_�V�F�[�_�v���O�����ւ̑g�ݍ���
    glAttachShader(program, vertShader);
    glDeleteShader(vertShader);

    if (frag)
    {
      // �t���O�����g�V�F�[�_�̍쐬
      GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
      
      // �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̓ǂݍ���
      if (readShaderSource(fragShader, frag))
      {
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // �t���O�����g�V�F�[�_�̃R���p�C��
      glCompileShader(fragShader);
      glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
      printShaderInfoLog(fragShader);
      if (compiled == GL_FALSE)
      {
        std::cerr << "Error: Could not compile fragment shader source: " << frag << std::endl;
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // �t���O�����g�V�F�[�_�V�F�[�_�v���O�����ւ̑g�ݍ���
      glAttachShader(program, fragShader);
      glDeleteShader(fragShader);
    }

    if (geom)
    {
      // �W�I���g���V�F�[�_�̍쐬
      GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
      
      // �W�I���g���V�F�[�_�̃\�[�X�v���O�����̓ǂݍ���
      if (readShaderSource(geomShader, geom))
      {
        glDeleteShader(geomShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // �W�I���g���V�F�[�_�̃R���p�C��
      glCompileShader(geomShader);
      glGetShaderiv(geomShader, GL_COMPILE_STATUS, &compiled);
      printShaderInfoLog(geomShader);
      if (compiled == GL_FALSE)
      {
        std::cerr << "Error: Could not compile geometry shader source: " << geom << std::endl;
        glDeleteShader(geomShader);
        glDeleteProgram(program);
        return 0;
      }
      
      // �W�I���g���V�F�[�_�̃V�F�[�_�v���O�����ւ̑g�ݍ���
      glAttachShader(program, geomShader);
      glDeleteShader(geomShader);

      // �W�I���g���V�F�[�_�ɓ��͂����{�}�`�̎w��
      glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, input);
      
      // �W�I���g���V�F�[�_����o�͂����{�}�`�̎w��
      glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);

      // �W�I���g���V�F�[�_���o�͂��钸�_���̐ݒ�
      if (vertices > 0) glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
    }

    // feedback �Ɏg�� varying �ϐ����w�肷��
    if (nvarying > 0) glTransformFeedbackVaryings(program, nvarying, varyings, GL_SEPARATE_ATTRIBS);

    // �V�F�[�_�v���O�����̃����N
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    printProgramInfoLog(program);
    if (linked == GL_FALSE)
    {
      std::cerr << "Error: Could not link shader program" << std::endl;
      glDeleteProgram(program);
      return 0;
    }
  }

  return program;
}

/*
** �e�N�X�`���}�b�s���O�p�� RAW �摜�t�@�C���̓ǂݍ���
*/
void gg::loadImage(const char *name, int width, int height, GLenum format)
{
  // �e�N�X�`���̓ǂݍ��ݐ�
  char *image = 0;
  
  // �e�N�X�`���t�@�C�����J��
  std::ifstream file(name, std::ios::binary);

  if (file.fail())
  {
    // �J���Ȃ�����
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else
  {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());

    // �e�N�X�`���T�C�Y���̃��������m�ۂ���
    GLsizei maxsize = width * height * ((format == GL_RGB) ? 3 : 4);
    image = new char[maxsize];

    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(image, (size < maxsize) ? size : maxsize);

    if (file.bad())
    {
      // ���܂��ǂݍ��߂Ȃ�����
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
 }
  
  // format �� RGBA �Ȃ� 4 �o�C�g���E�ɐݒ�
  glPixelStorei(GL_UNPACK_ALIGNMENT, (format == GL_RGBA) ? 4 : 1);
  
  // �e�N�X�`�������蓖�Ă�
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
  
  // �o�C���j�A�i�~�b�v�}�b�v�Ȃ��j�C�G�b�W�ŃN�����v
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  // �ǂݍ��݂Ɏg�������������J������
  delete[] image;
}

/*
** �����}�b�v�p�� RAW �摜�t�@�C���̓ǂݍ���Ŗ@���}�b�v���쐬����
*/
void gg::loadHeight(const char *name, int width, int height, float nz)
{
  // �쐬����@���}�b�v
  GLfloat (*nmap)[4] = 0;
  
  // �e�N�X�`���t�@�C�����J��
  std::ifstream file(name, std::ios::binary);
  
  if (file.fail())
  {
    // �J���Ȃ�����
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else
  {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());
    
    // ���������m�ۂ���
    GLsizei maxsize = width * height;
    GLubyte *hmap = 0;
    try {
      hmap = new GLubyte[maxsize];
      nmap = new GLfloat[maxsize][4];
    }
    catch (std::bad_alloc e) {
      delete[] hmap;
      delete[] nmap;
      throw e;
    }
    
    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(reinterpret_cast<char *>(hmap), (size < maxsize) ? size : maxsize);
    
    if (file.bad())
    {
      // ���܂��ǂݍ��߂Ȃ�����
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
    
    // �@���}�b�v�̍쐬
    for (int i = 0; i < maxsize; ++i)
    {
      int x = i % width, y = i - x;
      
      // �אڂ����f�Ƃ̒l�̍���@���x�N�g���̐����ɗp����
      float nx = static_cast<float>(hmap[y + (x + 1) % width] - hmap[i]);
      float ny = static_cast<float>(hmap[(y + width) % maxsize + x] - hmap[i]);
      
      // �@���x�N�g���̒��������߂Ă���
      float nl = sqrt(nx * nx + ny * ny + nz * nz);
      
      // �@���x�N�g�������߂�
      nmap[i][0] = nx * 0.5f / nl + 0.5f;
      nmap[i][1] = ny * 0.5f / nl + 0.5f;
      nmap[i][2] = nz * 0.5f / nl + 0.5f;
      nmap[i][3] = hmap[i] * 0.0039215686f; // = 1/255
    }

    // �����}�b�v�̓ǂݍ��݂Ɏg�������������J������
    delete[] hmap;
  }
  
  // nmap �� GLfloat �Ȃ̂� 4 �o�C�g���E�ɐݒ�
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  // �e�N�X�`�������蓖�Ă�
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nmap);
  
  // �o�C���j�A�i�~�b�v�}�b�v�Ȃ��j�C�G�b�W�ŃN�����v
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  
  // �@���}�b�v�̍쐬�Ɏg�������������������
  delete[] nmap;
}

/*
** �ϊ��s��F�s��ƃx�N�g���̐� c �� a �~ b
*/
void gg::GgMatrix::projection(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 4; ++i)
  {
    c[i] = a[0 + i] * b[0] + a[4 + i] * b[1] + a[8 + i] * b[2] + a[12 + i] * b[3];
  }
}

/*
** �ϊ��s��F�s��ƍs��̐� c �� a �~ b
*/
void gg::GgMatrix::multiply(GLfloat *c, const GLfloat *a, const GLfloat *b) const
{
  for (int i = 0; i < 16; ++i)
  {
    int j = i & 3, k = i & ~3;
    
    c[i] = a[0 + j] * b[k + 0] + a[4 + j] * b[k + 1] + a[8 + j] * b[k + 2] + a[12 + j] * b[k + 3];
  }
}

/*
** �ϊ��s��F�P�ʍs���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadIdentity(void)
{
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[ 0] = array[ 5] = array[10] = array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��F���s�ړ��ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadTranslate(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  array[12] = x;
  array[13] = y;
  array[14] = z;
  array[ 0] = array[ 5] = array[10] = array[15] = w;
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = 0.0f;
  
  return *this;
}

/*
** �ϊ��s��F�g��k���ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadScale(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  array[ 0] = x;
  array[ 5] = y;
  array[10] = z;
  array[15] = w;
  array[ 1] = array[ 2] = array[ 3] = array[ 4] =
  array[ 6] = array[ 7] = array[ 8] = array[ 9] =
  array[11] = array[12] = array[13] = array[14] = 0.0f;
  
  return *this;
}

/*
** �ϊ��s��Fx �����S�̉�]�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadRotateX(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = 1.0f; array[ 1] = 0.0f; array[ 2] = 0.0f; array[ 3] = 0.0f;
  array[ 4] = 0.0f; array[ 5] = c;    array[ 6] = s;    array[ 7] = 0.0f;
  array[ 8] = 0.0f; array[ 9] = -s;   array[10] = c;    array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��Fy �����S�̉�]�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadRotateY(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = c;    array[ 1] = 0.0f; array[ 2] = -s;   array[ 3] = 0.0f;
  array[ 4] = 0.0f; array[ 5] = 1.0f; array[ 6] = 0.0f; array[ 7] = 0.0f;
  array[ 8] = s;    array[ 9] = 0.0f; array[10] = c;    array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��Fz �����S�̉�]�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadRotateZ(GLfloat a)
{
  GLfloat c = cos(a);
  GLfloat s = sin(a);
  
  array[ 0] = c;    array[ 1] = s;    array[ 2] = 0.0f; array[ 3] = 0.0f;
  array[ 4] = -s;   array[ 5] = c;    array[ 6] = 0.0f; array[ 7] = 0.0f;
  array[ 8] = 0.0f; array[ 9] = 0.0f; array[10] = 1.0f; array[11] = 0.0f;
  array[12] = 0.0f; array[13] = 0.0f; array[14] = 0.0f; array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��F�C�ӎ����S�̉�]�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  GLfloat d = sqrt(x * x + y * y + z * z);
  
  if (d > 0.0f)
  {
    GLfloat l  = x / d, m  = y / d, n  = z / d;
    GLfloat l2 = l * l, m2 = m * m, n2 = n * n;
    GLfloat lm = l * m, mn = m * n, nl = n * l;
    GLfloat c = cos(a), c1 = 1.0f - c;
    GLfloat s = sin(a);
    
    array[ 0] = (1.0f - l2) * c + l2;
    array[ 1] = lm * c1 + n * s;
    array[ 2] = nl * c1 - m * s;
    array[ 3] = 0.0f;
    
    array[ 4] = lm * c1 - n * s;
    array[ 5] = (1.0f - m2) * c + m2;
    array[ 6] = mn * c1 + l * s;
    array[ 7] = 0.0f;
    
    array[ 8] = nl * c1 + m * s;
    array[ 9] = mn * c1 - l * s;
    array[10] = (1.0f - n2) * c + n2;
    array[11] = 0.0f;
    
    array[12] = 0.0f;
    array[13] = 0.0f;
    array[14] = 0.0f;
    array[15] = 1.0f;
  }
  
  return *this;
}

/*
** �ϊ��s��F�]�u�s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadTranspose(const GgMatrix &m)
{
  array[ 0] = m.array[ 0];
  array[ 1] = m.array[ 4];
  array[ 2] = m.array[ 8];
  array[ 3] = m.array[12];
  array[ 4] = m.array[ 1];
  array[ 5] = m.array[ 5];
  array[ 6] = m.array[ 9];
  array[ 7] = m.array[13];
  array[ 8] = m.array[ 2];
  array[ 9] = m.array[ 6];
  array[10] = m.array[10];
  array[11] = m.array[14];
  array[12] = m.array[ 3];
  array[13] = m.array[ 7];
  array[14] = m.array[11];
  array[15] = m.array[15];
  
  return *this;
}

/*
** �ϊ��s��F�t�s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadInvert(const GgMatrix &m)
{
  GLfloat lu[20], *plu[4];
  const GLfloat *marray = m.array;
  
  // j �s�̗v�f�̒l�̐�Βl�̍ő�l�� plu[j][4] �ɋ��߂�
  for (int j = 0; j < 4; ++j)
  {
    GLfloat max = fabs(*(plu[j] = lu + 5 * j) = *(marray++));
    
    for (int i = 0; ++i < 4;)
    {
      GLfloat a = fabs(plu[j][i] = *(marray++));
      if (a > max) max = a;
    }
    if (max == 0.0f) return *this;
    plu[j][4] = 1.0f / max;
  }
  
  // �s�{�b�g���l������ LU ����
  for (int j = 0; j < 4; ++j)
  {
    GLfloat max = fabs(plu[j][j] * plu[j][4]);
    int i = j;
    
    for (int k = j; ++k < 4;)
    {
      GLfloat a = fabs(plu[k][j] * plu[k][4]);
      if (a > max)
      {
        max = a;
        i = k;
      }
    }
    if (i > j)
    {
      GLfloat *t = plu[j];
      plu[j] = plu[i];
      plu[i] = t;
    }
    if (plu[j][j] == 0.0f) return *this;
    for (int k = j; ++k < 4;)
    {
      plu[k][j] /= plu[j][j];
      for (int i = j; ++i < 4;)
      {
        plu[k][i] -= plu[j][i] * plu[k][j];
      }
    }
  }
  
  // LU ��������t�s������߂�
  for (int k = 0; k < 4; ++k)
  {
    // array �ɒP�ʍs���ݒ肷��
    for (int i = 0; i < 4; ++i)
    {
      array[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0f : 0.0f;
    }
    // lu ����t�s������߂�
    for (int i = 0; i < 4; ++i)
    {
      for (int j = i; ++j < 4;)
      {
        array[j * 4 + k] -= array[i * 4 + k] * plu[j][i];
      }
    }
    for (int i = 4; --i >= 0;)
    {
      for (int j = i; ++j < 4;)
      {
        array[i * 4 + k] -= plu[i][j] * array[j * 4 + k];
      }
      array[i * 4 + k] /= plu[i][i];
    }
  }
  
  return *this;
}

/*
** �ϊ��s��F�@���ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadNormal(const GgMatrix &m)
{
  array[ 0] = m.array[ 5] * m.array[10] - m.array[ 6] * m.array[ 9];
  array[ 1] = m.array[ 6] * m.array[ 8] - m.array[ 4] * m.array[10];
  array[ 2] = m.array[ 4] * m.array[ 9] - m.array[ 5] * m.array[ 8];
  array[ 4] = m.array[ 9] * m.array[ 2] - m.array[10] * m.array[ 1];
  array[ 5] = m.array[10] * m.array[ 0] - m.array[ 8] * m.array[ 2];
  array[ 6] = m.array[ 8] * m.array[ 1] - m.array[ 9] * m.array[ 0];
  array[ 8] = m.array[ 1] * m.array[ 6] - m.array[ 2] * m.array[ 5];
  array[ 9] = m.array[ 2] * m.array[ 4] - m.array[ 0] * m.array[ 6];
  array[10] = m.array[ 0] * m.array[ 5] - m.array[ 1] * m.array[ 4];
  array[ 3] = array[ 7] = array[11] = array[12] = array[13] = array[14] = 0.0f;
  array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��F����ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadLookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  GLfloat l;
  
  // z �� = e - t
  tx = ex - tx;
  ty = ey - ty;
  tz = ez - tz;
  l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 2] = tx / l;
  array[ 6] = ty / l;
  array[10] = tz / l;
  
  // x �� = u x z ��
  tx = uy * array[10] - uz * array[ 6];
  ty = uz * array[ 2] - ux * array[10];
  tz = ux * array[ 6] - uy * array[ 2];
	l = sqrt(tx * tx + ty * ty + tz * tz);
  if (l == 0.0f) return *this;
  array[ 0] = tx / l;
  array[ 4] = ty / l;
  array[ 8] = tz / l;
  
  // y �� = z �� x x ��
  array[ 1] = array[ 6] * array[ 8] - array[10] * array[ 4];
  array[ 5] = array[10] * array[ 0] - array[ 2] * array[ 8];
  array[ 9] = array[ 2] * array[ 4] - array[ 6] * array[ 0];
  
  // ���s�ړ�
  array[12] = -(ex * array[ 0] + ey * array[ 4] + ez * array[ 8]);
  array[13] = -(ex * array[ 1] + ey * array[ 5] + ez * array[ 9]);
  array[14] = -(ex * array[ 2] + ey * array[ 6] + ez * array[10]);
  
  // �c��
  array[ 3] = array[ 7] = array[11] = 0.0f;
  array[15] = 1.0f;
  
  return *this;
}

/*
** �ϊ��s��F���s���e�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadOrthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    array[ 0] =  2.0f / dx;
    array[ 5] =  2.0f / dy;
    array[10] = -2.0f / dz;
    array[12] = -(right + left) / dx;
    array[13] = -(top + bottom) / dy;
    array[14] = -(zFar + zNear) / dz;
    array[15] =  1.0f;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] = array[11] = 0.0f;
  }
  
  return *this;
}

/*
** �ϊ��s��F�������e�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GLfloat dx = right - left;
  GLfloat dy = top - bottom;
  GLfloat dz = zFar - zNear;
  
  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    array[ 0] =  2.0f * zNear / dx;
    array[ 5] =  2.0f * zNear / dy;
    array[ 8] =  (right + left) / dx;
    array[ 9] =  (top + bottom) / dy;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[12] = array[13] = array[15] = 0.0f;
  }
  
  return *this;
}

/*
** �ϊ��s��F��p���瓧�����e�ϊ��s���ݒ肷��
*/
gg::GgMatrix &gg::GgMatrix::loadPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  GLfloat dz = zFar - zNear;
  
  if (dz != 0.0f)
  {
    GLfloat f = 1.0f / tan(fovy * 0.5f);
    
    array[ 0] = f / aspect;
    array[ 5] = f;
    array[10] = -(zFar + zNear) / dz;
    array[11] = -1.0f;
    array[14] = -2.0f * zFar * zNear / dz;
    array[ 1] = array[ 2] = array[ 3] = array[ 4] =
    array[ 6] = array[ 7] = array[ 8] = array[ 9] =
    array[12] = array[13] = array[15] = 0.0f;
  }
  
  return *this;
}

/*
** �ϊ��s��F����ϊ��s����悶��i���_�̈ړ��j
*/
gg::GgMatrix &gg::GgMatrix::lookat(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat ux, GLfloat uy, GLfloat uz)
{
  GgMatrix m;
  
  m.loadLookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
  multiply(m);
  
  return *this;
}

/*
** �ϊ��s��F���s���e�ϊ��s����悶��
*/
gg::GgMatrix &gg::GgMatrix::orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadOrthogonal(left, right, bottom, top, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** �ϊ��s��F�������e�ϊ��s����悶��
*/
gg::GgMatrix &gg::GgMatrix::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadFrustum(left, right, bottom, top, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** �ϊ��s��F��p���狁�߂��������e�ϊ��s����悶��
*/
gg::GgMatrix &gg::GgMatrix::perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
  GgMatrix m;
  
  m.loadPerspective(fovy, aspect, zNear, zFar);
  multiply(m);
  
  return *this;
}

/*
** �l�����F�l���� p, q �̘a�� r �ɋ��߂�
*/
void gg::GgQuaternion::add(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[0] + q[0];
  r[1] = p[1] + q[1];
  r[2] = p[2] + q[2];
  r[3] = p[3] + q[3];
}

/*
** �l�����F�l���� p, q �̍��� r �ɋ��߂�
*/
void gg::GgQuaternion::subtract(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[0] - q[0];
  r[1] = p[1] - q[1];
  r[2] = p[2] - q[2];
  r[3] = p[3] - q[3];
}

/*
** �l�����F�l���� p, q �̐ς� r �ɋ��߂�
*/
void gg::GgQuaternion::multiply(GLfloat *r, const GLfloat *p, const GLfloat *q) const
{
  r[0] = p[1] * q[2] - p[2] * q[1] + p[0] * q[3] + p[3] * q[0];
  r[1] = p[2] * q[0] - p[0] * q[2] + p[1] * q[3] + p[3] * q[1];
  r[2] = p[0] * q[1] - p[1] * q[0] + p[2] * q[3] + p[3] * q[2];
  r[3] = p[3] * q[3] - p[0] * q[0] - p[1] * q[1] - p[2] * q[2];
}

/*
** �l�����F�l���� q ���\���ϊ��s��� m �ɋ��߂�
*/
void gg::GgQuaternion::toMatrix(GLfloat *m, const GLfloat *q) const
{
  GLfloat xx = q[0] * q[0] * 2.0f;
  GLfloat yy = q[1] * q[1] * 2.0f;
  GLfloat zz = q[2] * q[2] * 2.0f;
  GLfloat xy = q[0] * q[1] * 2.0f;
  GLfloat yz = q[1] * q[2] * 2.0f;
  GLfloat zx = q[2] * q[0] * 2.0f;
  GLfloat xw = q[0] * q[3] * 2.0f;
  GLfloat yw = q[1] * q[3] * 2.0f;
  GLfloat zw = q[2] * q[3] * 2.0f;
  
  m[ 0] = 1.0f - yy - zz;
  m[ 1] = xy + zw;
  m[ 2] = zx - yw;
  m[ 4] = xy - zw;
  m[ 5] = 1.0f - zz - xx;
  m[ 6] = yz + xw;
  m[ 8] = zx + yw;
  m[ 9] = yz - xw;
  m[10] = 1.0f - xx - yy;
  m[ 3] = m[ 7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
  m[15] = 1.0f;
}

/*
** �l�����F��]�ϊ��s�� m ���\���l������ q �ɋ��߂�
*/
void gg::GgQuaternion::toQuaternion(GLfloat *q, const GLfloat *m) const
{
  GLfloat tr = m[0] + m[5] + m[10] + m[15];
  
  if (tr > 0.0f)
  {
    q[3] = sqrt(tr) * 0.5f;
    q[0] = (m[6] - m[9]) * 0.25f / q[3];
    q[1] = (m[8] - m[2]) * 0.25f / q[3];
    q[2] = (m[1] - m[4]) * 0.25f / q[3];
  }
}

/*
** �l�����F���ʐ��`��� p �� q �� r �� t �ŕ�Ԃ����l���������߂�
*/
void gg::GgQuaternion::slerp(GLfloat *p, const GLfloat *q, const GLfloat *r, GLfloat t) const
{
  GLfloat qr = q[0] * r[0] + q[1] * r[1] + q[2] * r[2] + q[3] * r[3];
  GLfloat ss = 1.0f - qr * qr;
  
  if (ss == 0.0f)
  {
    if (p != q)
    {
      p[0] = q[0];
      p[1] = q[1];
      p[2] = q[2];
      p[3] = q[3];
    }
  }
  else
  {
    GLfloat sp = sqrt(ss);
    GLfloat ph = acos(qr);
    GLfloat pt = ph * t;
    GLfloat t1 = sin(pt) / sp;
    GLfloat t0 = sin(ph - pt) / sp;
    
    p[0] = q[0] * t0 + r[0] * t1;
    p[1] = q[1] * t0 + r[1] * t1;
    p[2] = q[2] * t0 + r[2] * t1;
    p[3] = q[3] * t0 + r[3] * t1;
  }
}

/*
** �l�����F(x, y, z) �����Ƃ��p�x a ��]����l���������߂�
*/
gg::GgQuaternion &gg::GgQuaternion::loadRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
  GLfloat l = x * x + y * y + z * z;
  
  if (l != 0.0)
  {
    GLfloat s = sin(a *= 0.5f) / sqrt(l);
    
    array[0] = x * s;
    array[1] = y * s;
    array[2] = z * s;
  }
  else
  {
    array[0] = array[1] = array[2] = 0.0f;
  }
  array[3] = cos(a);
  
  return *this;
}

/*
** �l�����F�I�C���[�p (h, p, r) �ɂ��ƂÂ��Ďl���������߂�
*/
gg::GgQuaternion &gg::GgQuaternion::loadEuler(GLfloat h, GLfloat p, GLfloat r)
{
  return loadRotate(-sin(h) * cos(p), sin(p), cos(h) * cos(p), r);
}

/*
** �l�����F�m����
*/
GLfloat gg::GgQuaternion::norm(void) const
{
  return sqrt(array[0] * array[0] + array[1] * array[1] + array[2] * array[2] + array[3] * array[3]);
}

/*
** �l�����F�l���� q �̋����� r �ɋ��߂�
*/
gg::GgQuaternion gg::GgQuaternion::conjugate(void) const
{
  GgQuaternion t(-this->array[0], -this->array[1], -this->array[2], this->array[3]);
  
  return t;
}

/*
** �l�����F�l���� q �̋t�� r �ɋ��߂�
*/
gg::GgQuaternion gg::GgQuaternion::invert(void) const
{
  GgQuaternion t = this->conjugate();
  GLfloat l = this->array[0] * this->array[0] + this->array[1] * this->array[1]
  + this->array[2] * this->array[2] + this->array[3] * this->array[3];
  
  if (l > 0.0f)
  {
    t.array[0] /= l;
    t.array[1] /= l;
    t.array[2] /= l;
    t.array[3] /= l;
  }
  
  return t;
}

/*
** �l�����F���K��
*/
gg::GgQuaternion gg::GgQuaternion::normalize(void) const
{
  GgQuaternion t = *this;
  GLfloat l = this->norm();
  
  if (l > 0.0f)
  {
    t.array[0] /= l;
    t.array[1] /= l;
    t.array[2] /= l;
    t.array[3] /= l;
  }
  
  return t;
}

/*
** �ȈՃg���b�N�{�[�������F�R���X�g���N�^
*/
gg::GgTrackball::GgTrackball(void)
{
  // �h���b�O���ł͂Ȃ�
  drag = false;
  
  // �P�ʃN�H�[�^�[�j�I��
  cq.loadIdentity();
  
  // ��]�s��̏�����
  cq.getMatrix(rt);
}

/*
** �ȈՃg���b�N�{�[�������F�g���b�N�{�[������̈�̐ݒ�
**
**    Reshape �R�[���o�b�N (resize) �̒��Ŏ��s����
**    (w, h): �E�B���h�E�T�C�Y
*/
void gg::GgTrackball::region(int w, int h)
{
  // �}�E�X�|�C���^�ʒu�̃E�B���h�E���̑��ΓI�ʒu�ւ̊��Z�p
  sx = 1.0f / (float)w;
  sy = 1.0f / (float)h;
}

/*
** �ȈՃg���b�N�{�[�������F�h���b�O�J�n���̏���
**
**    �}�E�X�{�^�����������Ƃ��Ɏ��s����
**    (x, y): ���݂̃}�E�X�ʒu
*/
void gg::GgTrackball::start(int x, int y)
{
  // �h���b�O�J�n
  drag = true;
  
  // �h���b�O�J�n�_���L�^����
  cx = x;
  cy = y;
}

/*
** �ȈՃg���b�N�{�[�������F�h���b�O���̏���
**
**    �}�E�X�̃h���b�O���Ɏ��s����
**    (x, y): ���݂̃}�E�X�ʒu
*/
void gg::GgTrackball::motion(int x, int y)
{
  if (drag)
  {
    float dx, dy, a;
    
    // �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ�
    dx = (x - cx) * sx;
    dy = (y - cy) * sy;
    
    // �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋���
    a = sqrt(dx * dx + dy * dy);
    
    if (a != 0.0)
    {
      // ��]���Ɖ�]�p����l���������
      GgQuaternion dq;
      dq.loadRotate(dy, dx, 0.0f, a * 6.283185f);
      
      // ���݂̉�]�̎l�����ɍ�����l�������|���č�������
      tq = dq * cq;
      
      // ���������l���������]�̕ϊ��s������߂�
      tq.getMatrix(rt);
    }
  }
}

/*
** �ȈՃg���b�N�{�[�������F��~���̏���
**
**    �}�E�X�{�^���𗣂����Ƃ��Ɏ��s����
**    (x, y): ���݂̃}�E�X�ʒu
*/
void gg::GgTrackball::stop(int x, int y)
{
  // �h���b�O�I���_�ɂ������]�����߂�
  motion(x, y);
  
  // ���݂̉�]��\���l�����𐳋K�����ĕۑ�����
  cq = tq.normalize();
  
  // �h���b�O�I��
  drag = false;
}

/*
** �_�Q�F�R�s�[
*/
void gg::GgPoints::copy(const GgPoints &o)
{
  if (o.nv > 0)
  {
    // �������̊m��
    GLfloat (*vert)[3] = new GLfloat[o.nv][3];
    
    // �f�[�^�̃R�s�[
    memcpy(vert, o.vert, sizeof (GLfloat[3]) * o.nv);
    
    // �f�[�^�̓o�^
    entry(o.nv, vert);
  }
}

/*
** �_�Q�F�`��
*/
void gg::GgPoints::draw(void) const
{
  // �V�F�[�_�[�̑I��
  getShader()->use(vert);
  
  // ���_�z��Ő}�`��`�悷��
  glDrawArrays(GL_POINTS, 0, nv);
}

/*
** ���b�V���F�R�s�[
*/
void gg::GgPolygon::copy(const GgPolygon &o)
{
  if (o.getNv() > 0)
  {
    // �������̊m��
    GLfloat (*norm)[3] = new GLfloat[o.getNv()][3];
    
    // �f�[�^�̃R�s�[
    memcpy(norm, o.norm, sizeof (GLfloat[3]) * o.getNv());
    
    // �f�[�^�̓o�^
    entry(norm);
  }
}

/*
** ���b�V���F�`��
*/
void gg::GgPolygon::draw(void) const
{
  // �V�F�[�_�[�̑I��
  getShader()->use(getVert(), norm);
  
  // ���_�z��Ő}�`��`�悷��
  glDrawArrays(GL_TRIANGLE_FAN, 0, getNv());
}

/*
** �I�u�W�F�N�g�F�R�s�[
*/
void gg::GgObject::copy(const GgObject &o)
{
  if (o.nf > 0)
  {
    // �������̊m��
    GLuint (*face)[3] = new GLuint[o.nf][3];
    
    // �f�[�^�̃R�s�[
    memcpy(face, o.face, sizeof (GLuint[3]) * o.nf);
    
    // �f�[�^�̓o�^
    entry(o.nf, face);
  }
}

/*
** �I�u�W�F�N�g�F�`��
*/
void gg::GgObject::draw(void) const
{
  // �V�F�[�_�v���O�����̑I��
  getShader()->use(getVert(), getNorm());
  
  // ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��
  glDrawElements(GL_TRIANGLES, getNf() * 3, GL_UNSIGNED_INT, getFace());
}

/*
** ����̓_�Q
*/
gg::GgPoints *gg::ggPointSphere(GLuint nv, GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius)
{
  // �������̊m��
  GLfloat (*vert)[3] = new GLfloat[nv][3];
  
  // �_�̐���
  for (GLuint v = 0; v < nv; ++v)
  {
    float r = radius * (float)rand() / (float)RAND_MAX;
    float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
    float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
    float sp = sqrt(1.0f - cp * cp);
    float ct = cos(t), st = sin(t);
    
    vert[v][0] = r * sp * ct + cx;
    vert[v][1] = r * sp * st + cy;
    vert[v][2] = r * cp + cz;
  }
  
  return new gg::GgPoints(nv, vert);
}

/*
** ��`
*/
gg::GgPolygon *gg::ggRectangle(GLfloat width, GLfloat height)
{
  // ��ƂȂ�`��
  static const GLfloat p[][2] =
  {
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
  };
  
  // �������̊m��
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  try
  {
    vert = new GLfloat[4][3];
    norm = new GLfloat[4][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    throw e;
  }

  for (int v = 0; v < 4; ++v)
  {
    vert[v][0] = p[v][0] * width;
    vert[v][1] = p[v][1] * height;
    vert[v][2] = 0.0f;
    
    norm[v][0] = 0.0f;
    norm[v][1] = 0.0f;
    norm[v][2] = 1.0f;
  }
  
  return new gg::GgPolygon(4, vert, norm);
}

/*
** �ȉ~
*/
gg::GgPolygon *gg::ggEllipse(GLfloat width, GLfloat height, GLuint slices)
{
  // �������̊m��
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  try
  {
    vert = new GLfloat[slices][3];
    norm = new GLfloat[slices][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    throw e;
  }
  
  for (GLuint v = 0; v < slices; ++v)
  {
    float t = 6.2831853f * (float)v / (float)slices;
    
    vert[v][0] = cos(t) * width * 0.5f;
    vert[v][1] = sin(t) * height * 0.5f;
    vert[v][2] = 0.0f;
    
    norm[v][0] = 0.0f;
    norm[v][1] = 0.0f;
    norm[v][2] = 1.0f;
  }
  
  return new gg::GgPolygon(slices, vert, norm);
}

/*
** �O�p�`�������ꂽ Alias OBJ �t�@�C��
*/
gg::GgObject *gg::ggObj(const char *name, bool normalize)
{
  // �t�@�C���̓ǂݍ���
  std::ifstream file(name, std::ios::binary);
  if (file.fail())
  {
    std::cerr << "Can't open file: " << name << std::endl;
    return 0;
  }
  
  // �f�[�^�̐��ƍ��W�l�̍ŏ��l�E�ő�l�𒲂ׂ�
  char buf[1024];
  int nv, nf;
  nv = nf = 0;
  float xmin, xmax, ymin, ymax, zmin, zmax;
  xmax = ymax = zmax = -(xmin = ymin = zmin = FLT_MAX);
  while (file.getline(buf, sizeof buf))
  {
    if (buf[0] == 'v' && buf[1] == ' ')
    {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      if (x < xmin) xmin = x;
      if (x > xmax) xmax = x;
      if (y < ymin) ymin = y;
      if (y > ymax) ymax = y;
      if (z < zmin) zmin = z;
      if (z > zmax) zmax = z;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ')
    {
      ++nf;
    }
  }
  
  // �������̊m��
  GLfloat (*vert)[3] = 0;
  GLfloat (*norm)[3] = 0;
  GLfloat (*fnorm)[3] = 0;
  GLuint (*face)[3] = 0;
  try
  {
    vert = new GLfloat[nv][3];
    norm = new GLfloat[nv][3];
    fnorm = new GLfloat[nf][3];
    face = new GLuint[nf][3];
  }
  catch (std::bad_alloc e)
  {
    delete[] vert;
    delete[] norm;
    delete[] fnorm;
    delete[] face;
    throw e;
  }
  
  // ���K��
  GLfloat scale, cx, cy, cz;
  if (normalize)
  {
    float sx = xmax - xmin;
    float sy = ymax - ymin;
    float sz = zmax - zmin;
    scale = sx;
    if (sy > scale) scale = sy;
    if (sz > scale) scale = sz;
    scale = (scale != 0.0f) ? 1.0f / scale : 1.0f;
    cx = (xmax + xmin) * 0.5f;
    cy = (ymax + ymin) * 0.5f;
    cz = (zmax + zmin) * 0.5f;
  }    
  else {
    scale = 1.0f;
    cx = cy = cz = 0.0f;
  }
  
  // �t�@�C���̊����߂�
  file.clear();
  file.seekg(0L, std::ios::beg);
  
  // �f�[�^�̓ǂݍ���
  nv = nf = 0;
  while (file.getline(buf, sizeof buf))
  {
    if (buf[0] == 'v' && buf[1] == ' ')
    {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      vert[nv][0] = (x - cx) * scale;
      vert[nv][1] = (y - cy) * scale;
      vert[nv][2] = (z - cz) * scale;
      ++nv;
    }
    else if (buf[0] == 'f' && buf[1] == ' ')
    {
      if (sscanf(buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3)
        if (sscanf(buf + 2, "%d//%*d %d//%*d %d//%*d", face[nf], face[nf] + 1, face[nf] + 2) != 3)
          sscanf(buf + 2, "%d %d %d", face[nf], face[nf] + 1, face[nf] + 2);
      --face[nf][0];
      --face[nf][1];
      --face[nf][2];
      ++nf;
    }
  }
  
  // �ʖ@���x�N�g���̎Z�o
  for (int f = 0; f < nf; ++f)
  {
    GLfloat dx1 = vert[face[f][1]][0] - vert[face[f][0]][0];
    GLfloat dy1 = vert[face[f][1]][1] - vert[face[f][0]][1];
    GLfloat dz1 = vert[face[f][1]][2] - vert[face[f][0]][2];
    GLfloat dx2 = vert[face[f][2]][0] - vert[face[f][0]][0];
    GLfloat dy2 = vert[face[f][2]][1] - vert[face[f][0]][1];
    GLfloat dz2 = vert[face[f][2]][2] - vert[face[f][0]][2];
    
    // �O��
    fnorm[f][0] = dy1 * dz2 - dz1 * dy2;
    fnorm[f][1] = dz1 * dx2 - dx1 * dz2;
    fnorm[f][2] = dx1 * dy2 - dy1 * dx2;
  }
  
  // ���_�̖@���x�N�g���̒l�� 0 �ɂ��Ă���
  for (int v = 0; v < nv; ++v)
    norm[v][0] = norm[v][1] = norm[v][2] = 0.0f;
  
  // �ʂ̖@���x�N�g���𒸓_�̖@���x�N�g���ɐώZ����
  for (int f = 0; f < nf; ++f)
  {
    norm[face[f][0]][0] += fnorm[f][0];
    norm[face[f][0]][1] += fnorm[f][1];
    norm[face[f][0]][2] += fnorm[f][2];
    
    norm[face[f][1]][0] += fnorm[f][0];
    norm[face[f][1]][1] += fnorm[f][1];
    norm[face[f][1]][2] += fnorm[f][2];
    
    norm[face[f][2]][0] += fnorm[f][0];
    norm[face[f][2]][1] += fnorm[f][1];
    norm[face[f][2]][2] += fnorm[f][2];
  }
  
  // ���_�̖@���x�N�g���𐳋K������
  for (int v = 0; v < nv; ++v)
  {
    GLfloat a = sqrt(norm[v][0] * norm[v][0]
                   + norm[v][1] * norm[v][1]
                   + norm[v][2] * norm[v][2]);
    
    if (a != 0.0)
    {
      norm[v][0] /= a;
      norm[v][1] /= a;
      norm[v][2] /= a;
    }
  }
  
  // �ʂ̖@���x�N�g���͒��_�̖@���x�N�g���̎Z�o�ɂ����g��Ȃ��̂ŉ��
  delete[] fnorm;
  
  return new gg::GgObject(nv, vert, norm, nf, face);
}
