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

#ifndef __GG_H__
#define __GG_H__

#if defined(WIN32)
#  pragma warning(disable:4996)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  pragma comment(lib, "glew32.lib")
#  include "glew.h"
#  include "wglew.h"
#  include "glut.h"
#elif defined(X11)
#  define GL_GLEXT_PROTOTYPES
#  define GLX_GLXEXT_PROTOTYPES
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glx.h>
#  include <GL/glut.h>
#elif defined(__APPLE__)
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
// GL_ARB_texture_float
#  define GL_RGBA32F GL_RGBA32F_ARB
#  define GL_RGB32F GL_RGB32F_ARB
#  define GL_ALPHA32F GL_ALPHA32F_ARB
#  define GL_INTENSITY32F GL_INTENSITY32F_ARB
#  define GL_LUMINANCE32F GL_LUMINANCE32F_ARB
#  define GL_LUMINANCE_ALPHA32F GL_LUMINANCE_ALPHA32F_ARB
#  define GL_RGBA16F GL_RGBA16F_ARB
#  define GL_RGB16F GL_RGB16F_ARB
#  define GL_ALPHA16F GL_ALPHA16F_ARB
#  define GL_INTENSITY16F GL_INTENSITY16F_ARB
#  define GL_LUMINANCE16F GL_LUMINANCE16F_ARB
#  define GL_LUMINANCE_ALPHA16F GL_LUMINANCE_ALPHA16F_ARB
// GL_EXT_transform_feedback
#  define glBindBufferRange glBindBufferRangeEXT
#  define glBindBufferOffset glBindBufferOffsetEXT
#  define glBindBufferBase glBindBufferBaseEXT
#  define glBeginTransformFeedback glBeginTransformFeedbackEXT
#  define glEndTransformFeedback glEndTransformFeedbackEXT
#  define glTransformFeedbackVaryings glTransformFeedbackVaryingsEXT
#  define glGetTransformFeedbackVarying glGetTransformFeedbackVaryingEXT
#  define glGetIntegerIndexedv glGetIntegerIndexedvEXT
#  define glGetBooleanIndexedv glGetBooleanIndexedvEXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER GL_TRANSFORM_FEEDBACK_BUFFER_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_START GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT
#  define GL_INTERLEAVED_ATTRIBS GL_INTERLEAVED_ATTRIBS_EXT
#  define GL_SEPARATE_ATTRIBS GL_SEPARATE_ATTRIBS_EXT
#  define GL_PRIMITIVES_GENERATED GL_PRIMITIVES_GENERATED_EXT
#  define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT
#  define GL_RASTERIZER_DISCARD GL_RASTERIZER_DISCARD_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT
#  define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYINGS GL_TRANSFORM_FEEDBACK_VARYINGS_EXT
#  define GL_TRANSFORM_FEEDBACK_BUFFER_MODE GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT
#  define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT
// GL_APPLE_vertex_array_object
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#  define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#  define glIsVertexArray glIsVertexArrayAPPLE
#  define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_APPLE
#else
#  error "This platform is not supported."
#endif

#define GG_DEG_TO_RAD(a) ((a) * 0.017453293f)

namespace gg
{
  /*
  ** �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  **
  **    OpenGL �̃����_�����O�R���e�L�X�g���쐬��Ɏ��s����
  */
  extern void ggInit(void);

  /*
  ** OpenGL �̃G���[�`�F�b�N
  **
  **    OpenGL �� API ���Ăяo������Ɏ��s����΃G���[�̂���Ƃ��Ƀ��b�Z�[�W��\������
  **    msg �̓��b�Z�[�W�ɏ悶�镶����
  */
  extern void ggError(const char *msg = 0);

  /*
  ** FBO �̃G���[�`�F�b�N
  **
  **    FBO �� API ���Ăяo������Ɏ��s����΃G���[�̂���Ƃ��Ƀ��b�Z�[�W��\������
  **    msg �̓��b�Z�[�W�ɏ悶�镶����
  */
  extern void ggFBOError(const char *msg = 0);

  /*
  ** �V�F�[�_�[�\�[�X�t�@�C���̓ǂݍ���
  */
  extern GLuint loadShader(
    const char *vert,                   // �o�[�e�b�N�X�V�F�[�_�̃\�[�X�t�@�C����
    const char *frag = 0,               // �t���O�����g�V�F�[�_�̃\�[�X�t�@�C�����i0 �Ȃ�s�g�p�j
    const char *geom = 0,               // �W�I���g���V�F�[�_�̃\�[�X�t�@�C�����i0 �Ȃ�s�g�p�j
    GLenum input = GL_TRIANGLES,        // �W�I���g���V�F�[�_�̓��̓v���~�e�B�u
    GLenum output = GL_TRIANGLE_STRIP,  // �W�I���g���V�F�[�_�̏o�̓v���~�e�B�u
    int nvarying = 0,                   // �t�B�[�h�o�b�N���� varying �ϐ��̐��i0 �Ȃ�s�g�p�j
    const char **varyings = 0           // �t�B�[�h�o�b�N���� varying �ϐ��̃��X�g
  );

  /*
  ** �e�N�X�`���}�b�s���O�p�� RAW �摜�t�@�C���̓ǂݍ���
  */
  extern void loadImage(const char *name, int width, int height, GLenum format);

  /*
  ** �����}�b�v�p�� RAW �摜�t�@�C���̓ǂݍ���Ŗ@���}�b�v���쐬����
  */
  extern void loadHeight(const char *name, int width, int height, float nz);
}

#endif
