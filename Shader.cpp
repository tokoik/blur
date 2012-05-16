/*
** シェーダプログラム
*/
#include <iostream>
#include <fstream>

#include "Shader.h"

/*
** シェーダーのソースプログラムをメモリに読み込む
*/
static bool readShaderSource(GLuint shader, const char *name)
{
  bool ret = false; // 終了ステータス（false ならエラー）
  
  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  if (file.fail()) {
    // 開けなかった
    std::cerr << "Error: Can't open source file: " << name << std::endl;
  }
  else {
    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    
    // ファイルサイズのメモリを確保
    char *buffer = new GLchar[length];
    
    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(buffer, length);

    if (file.bad()) {
      // うまく読み込めなかった
      std::cerr << "Error: Could not read souce file: " << name << std::endl;
    }
    else {
      // シェーダのソースプログラムのシェーダオブジェクトへの読み込み
      const GLchar *source[] = { buffer };
      glShaderSource(shader, 1, source, &length);
      ret = true;
    }
    file.close();
    
    // 読み込みに使ったメモリを開放する
    delete[] buffer;
  }
  
  return ret;
}

/*
** シェーダの情報を表示する
*/
static void printShaderInfoLog(GLuint shader)
{
  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    // シェーダのコンパイル時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** プログラムの情報を表示する
*/
static void printProgramInfoLog(GLuint program)
{
  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    // シェーダのリンク時のログの内容を取得する
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}

/*
** シェーダーソースファイルの読み込み
*/
bool Shader::load(const char *vert, const char *frag, const char *geom)
{
  GLint compiled, linked; // コンパイル結果, リンク結果

  // プログラムオブジェクトの作成
  program = glCreateProgram();
  
  // バーテックスシェーダ
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  if (!readShaderSource(vertShader, vert)) return false;
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    std::cerr << "Error: Could not compile vertex shader source: " << vert << std::endl;
    return false;
  }
  glAttachShader(program, vertShader);
  glDeleteShader(vertShader);

  // フラグメントシェーダ
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!readShaderSource(fragShader, frag)) return false;
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    std::cerr << "Error: Could not compile fragment shader source: " << frag << std::endl;
    return false;
  }
  glAttachShader(program, fragShader);
  glDeleteShader(fragShader);

  // ジオメトリシェーダ（オプション）
  if (geom) {
    GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
    if (!readShaderSource(geomShader, geom)) return false;
    glCompileShader(geomShader);
    glGetShaderiv(geomShader, GL_COMPILE_STATUS, &compiled);
    printShaderInfoLog(geomShader);
    if (compiled == GL_FALSE) {
      std::cerr << "Error: Could not compile geometry shader source: " << geom << std::endl;
      return false;
    }
    glAttachShader(program, geomShader);
    glDeleteShader(geomShader);
  }
  
  // シェーダプログラムのリンク
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(program);
  if (linked == GL_FALSE) {
    std::cerr << "Error: Could not link shader program" << std::endl;
    return false;
  }

  return true;
}
