/*
** �V�F�[�_�v���O����
*/
#include <iostream>
#include <fstream>

#include "Shader.h"

/*
** �V�F�[�_�[�̃\�[�X�v���O�������������ɓǂݍ���
*/
static bool readShaderSource(GLuint shader, const char *name)
{
  bool ret = false; // �I���X�e�[�^�X�ifalse �Ȃ�G���[�j
  
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
      ret = true;
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
bool Shader::load(const char *vert, const char *frag, const char *geom)
{
  GLint compiled, linked; // �R���p�C������, �����N����

  // �v���O�����I�u�W�F�N�g�̍쐬
  program = glCreateProgram();
  
  // �o�[�e�b�N�X�V�F�[�_
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

  // �t���O�����g�V�F�[�_
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

  // �W�I���g���V�F�[�_�i�I�v�V�����j
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
  
  // �V�F�[�_�v���O�����̃����N
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(program);
  if (linked == GL_FALSE) {
    std::cerr << "Error: Could not link shader program" << std::endl;
    return false;
  }

  return true;
}
