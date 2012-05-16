#ifndef TEXTURE_H
#define TEXTURE_H

/*
** テクスチャ
*/
#include "Attribute.h"

class Texture : public Attribute {
  GLuint name;  // テクスチャ名
public:
  Texture(void) { glGenTextures(1, &name); }
  virtual ~Texture(void) { if (name) glDeleteTextures(1, &name), name = 0; }
  void loadImage(const char *name, int width, int height, GLenum format = GL_RGB) const;
  void loadHeight(const char *name, int width, int height, float nz = 1.0f) const;
  void bind(void) const { glBindTexture(GL_TEXTURE_2D, name); }
  void unbind(void) const { glBindTexture(GL_TEXTURE_2D, 0); }
};

#endif
