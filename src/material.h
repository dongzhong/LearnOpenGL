// Created by Dong Zhong on 2022/02/27.

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <memory>

#include <glad/glad.h>

#include "shader.h"
#include "texture.h"

class Material {
 public:
  Material(float shininess = 32.0f, bool is_blinn_phong = true);

  void SetRenderShader(const std::shared_ptr<Shader>& shader);
  std::shared_ptr<Shader> GetRenderShader() const { return shader_; }

  float GetShininess() const { return shininess_; }
  void SetShinieness(float shininess);

  bool IsBlinnPhong() const { return is_blinn_phong_; }
  void SetBlinnPhong(bool blinn_phong);

 private:
  std::shared_ptr<Shader> shader_;

  float shininess_;
  bool is_blinn_phong_;
};

#endif // MATERIAL_H_
