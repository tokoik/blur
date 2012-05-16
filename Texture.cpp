/*
** テクスチャ
*/
#include <iostream>
#include <fstream>
#include <cmath>

#include "Texture.h"

/*
** テクスチャマッピング用の RAW 画像ファイルの読み込み
*/
void Texture::loadImage(const char *name, int width, int height, GLenum format) const
{
  // テクスチャの読み込み先
  char *image = 0;
  
  // テクスチャファイルを開く
  std::ifstream file(name, std::ios::binary);

  if (file.fail()) {
    // 開けなかった
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());

    // テクスチャサイズ分のメモリを確保する
    GLsizei maxsize = width * height * ((format == GL_RGB) ? 3 : 4);
    image = new char[maxsize];

    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(image, (size < maxsize) ? size : maxsize);

    if (file.bad()) {
      // うまく読み込めなかった
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
  }
  
  // format が RGBA なら 4 バイト境界に設定
  glPixelStorei(GL_UNPACK_ALIGNMENT, (format == GL_RGBA) ? 4 : 1);
  
  // テクスチャを割り当てる
  glBindTexture(GL_TEXTURE_2D, this->name);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
  
  // バイリニア（ミップマップなし），エッジでクランプ
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  // 読み込みに使ったメモリを開放する
  delete[] image;
}

/*
** 高さマップ用の RAW 画像ファイルの読み込んで法線マップを作成する
*/
void Texture::loadHeight(const char *name, int width, int height, float nz) const
{
  // 高さマップの読み込み先
  GLubyte *hmap = 0;
  
  // 法線マップ
  GLfloat (*nmap)[4] = 0;
  
  // テクスチャファイルを開く
  std::ifstream file(name, std::ios::binary);
  
  if (file.fail()) {
    // 開けなかった
    std::cerr << "Waring: Can't open texture file: " << name << std::endl;
  }
  else {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei size = static_cast<GLsizei>(file.tellg());
    
    // メモリを確保する
    GLsizei maxsize = width * height;
    hmap = new GLubyte[maxsize];
    nmap = new GLfloat[maxsize][4];
    
    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(reinterpret_cast<char *>(hmap), (size < maxsize) ? size : maxsize);
    
    if (file.bad()) {
      // うまく読み込めなかった
      std::cerr << "Warning: Could not read texture file: " << name << std::endl;
    }
    file.close();
    
    // 法線マップの作成
    for (int i = 0; i < maxsize; ++i) {
      int x = i % width, y = i - x;
      
      // 隣接する画素との値の差を法線ベクトルの成分に用いる
      float nx = static_cast<float>(hmap[y + (x + 1) % width] - hmap[i]);
      float ny = static_cast<float>(hmap[(y + width) % maxsize + x] - hmap[i]);
      
      // 法線ベクトルの長さを求めておく
      float nl = sqrt(nx * nx + ny * ny + nz * nz);
      
      nmap[i][0] = nx * 0.5f / nl + 0.5f;
      nmap[i][1] = ny * 0.5f / nl + 0.5f;
      nmap[i][2] = nz * 0.5f / nl + 0.5f;
      nmap[i][3] = hmap[i] * 0.0039215686f;
    }
  }
  
  // nmap が GLfloat なので 4 バイト境界に設定
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  // テクスチャを割り当てる
  glBindTexture(GL_TEXTURE_2D, this->name);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nmap);
  
  // バイリニア（ミップマップなし），エッジでクランプ
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  
  // 読み込みに使ったメモリを開放する
  delete[] hmap;
  delete[] nmap;
}
