// Created by Dong Zhong on 2022/02/24.

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path) {
  std::string direction(SHADER_PATH);
  std::string vs_path = direction + std::string(vertex_shader_path);
  std::string fs_path = direction + std::string(fragment_shader_path);

  std::string vertex_code;
  std::string fragment_code;

  std::ifstream vertex_file;
  std::ifstream fragment_file;

  vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertex_file.open(vs_path);
    fragment_file.open(fs_path);

    std::stringstream v_shader_stream, f_shader_stream;
    v_shader_stream << vertex_file.rdbuf();
    f_shader_stream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_code = v_shader_stream.str();
    fragment_code = f_shader_stream.str();
  } catch (std::ifstream::failure& e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
  }

  const char* v_shader_code = vertex_code.c_str();
  const char* f_shader_code = fragment_code.c_str();

  GLint success;
  char info_log[512];
  GLuint v_shader, f_shader;

  v_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_shader, 1, &v_shader_code, nullptr);
  glCompileShader(v_shader);

  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(v_shader, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Vertex shader compile error: " << info_log;
  }

  f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_shader_code, nullptr);
  glCompileShader(f_shader);

  glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(f_shader, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Vertex shader compile error: " << info_log;
  }

  program_ = glCreateProgram();
  glAttachShader(program_, v_shader);
  glAttachShader(program_, f_shader);
  glLinkProgram(program_);

  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Program link error: " << info_log;
  }

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
}

void Shader::Use() const {
  glUseProgram(program_);
}

void Shader::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) const {
  glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
