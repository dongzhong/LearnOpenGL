// Created by Dong Zhong on 2022/02/24.

#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SHADER_PATH "/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/shaders/"

class Shader {
 public:
  Shader(const char* vertex_shader_path, const char* fragment_shader_path);

  void Use() const;

  void SetBool(const std::string& name, bool value) const;

  void SetInt(const std::string& name, int value) const;

  void SetFloat(const std::string& name, float value) const;

  void SetVec2(const std::string& name, const glm::vec2& value) const;

  void SetVec2(const std::string& name, float x, float y) const;

  void SetVec3(const std::string& name, const glm::vec3& value) const;

  void SetVec3(const std::string& name, float x, float y, float z) const;

  void SetVec4(const std::string& name, const glm::vec4& value) const;

  void SetVec4(const std::string& name, float x, float y, float z, float w) const;

  void SetMat2(const std::string& name, const glm::mat2& value) const;

  void SetMat3(const std::string& name, const glm::mat3& value) const;

  void SetMat4(const std::string& name, const glm::mat4& value) const;

 private:
  GLuint program_;
};

#endif // SHADER_H_
