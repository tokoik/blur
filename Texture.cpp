/*
** �e�N�X�`��
*/
#include <iostream>
#include <fstream>
#include <cmath>

#include "Texture.h"

/*
** �e�N�X�`���}�b�s���O�p�� RAW �摜�t�@�C���̓ǂݍ���
*/
void Texture::loadImage(const char *name, int width, int height, GLenum format) const
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
  glBindTexture(GL_TEXTURE_2D, this->name);
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
void Texture::loadHeight(const char *name, int width, int height, float nz) const
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
      
      nmap[i][0] = nx * 0.5f / nl + 0.5f;
      nmap[i][1] = ny * 0.5f / nl + 0.5f;
      nmap[i][2] = nz * 0.5f / nl + 0.5f;
      nmap[i][3] = hmap[i] * 0.0039215686f;
    }
  }
  
  // nmap �� GLfloat �Ȃ̂� 4 �o�C�g���E�ɐݒ�
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  // �e�N�X�`�������蓖�Ă�
  glBindTexture(GL_TEXTURE_2D, this->name);
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
