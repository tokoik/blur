/*
** �Q�[���O���t�B�b�N�X���_�p�⏕�v���O����
**

Copyright (c) 2011 Kohe Tokoi. All Rights Reserved.

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
#  if !defined(__APPLE__)
  Display *dpy = glXGetCurrentDisplay();
  GLXDrawable drawable = glXGetCurrentDrawable();
  if (drawable)
  {
    glXSwapIntervalEXT(dpy, drawable, 1);
    return;
  }
#  endif
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

  if (file.fail()) {
    // �J���Ȃ�����
    std::cerr << "Error: Can't open source file: " << name << std::endl;
  }
  else {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    
    // �t�@�C���T�C�Y�̃��������m��
    char *buffer = new GLchar[length];
    
    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(buffer, length);

    if (file.bad()) {
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
  
  if (bufSize > 1) {
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
  
  if (bufSize > 1) {
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

      // �W�I���g���V�F�[�_���o�͂��钸�_�����o�͉\�Ȓl�ɐݒ肷��
      int vertices;
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &vertices);
      glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
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

  if (file.fail()) {
    // �J���Ȃ�����
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());

    // �e�N�X�`���T�C�Y���̃��������m�ۂ���
    GLsizei maxsize = width * height * ((format == GL_RGB) ? 3 : 4);
    image = new char[maxsize];

    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(image, (size < maxsize) ? size : maxsize);

    if (file.bad()) {
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
  // �����}�b�v�̓ǂݍ��ݐ�
  GLubyte *hmap = 0;
  
  // �@���}�b�v
  GLfloat (*nmap)[4] = 0;
  
  // �e�N�X�`���t�@�C�����J��
  std::ifstream file(name, std::ios::binary);
  
  if (file.fail()) {
    // �J���Ȃ�����
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else {
    // �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());
    
    // ���������m�ۂ���
    GLsizei maxsize = width * height;
    hmap = new GLubyte[maxsize];
    nmap = new GLfloat[maxsize][4];
    
    // �t�@�C����擪����ǂݍ���
    file.seekg(0L, std::ios::beg);
    file.read(reinterpret_cast<char *>(hmap), (size < maxsize) ? size : maxsize);
    
    if (file.bad()) {
      // ���܂��ǂݍ��߂Ȃ�����
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
    
    // �@���}�b�v�̍쐬
    for (int i = 0; i < maxsize; ++i) {
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
  
  // �ǂݍ��݂Ɏg�������������J������
  delete[] hmap;
  delete[] nmap;
}
