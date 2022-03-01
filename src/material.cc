// Created by Dong Zhong on 2022/02/27.

#include "material.h"

Material::Material(float shininess, bool is_blinn_phong)
    : shininess_(shininess),
      is_blinn_phong_(is_blinn_phong) {}

void Material::SetRenderShader(const std::shared_ptr<Shader>& shader) {
  shader_ = shader;
}

void Material::SetShinieness(float shininess) {
  shininess_ = shininess;
}

void Material::SetBlinnPhong(bool blinn_phong) {
  is_blinn_phong_ = blinn_phong;
}
