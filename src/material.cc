// Created by Dong Zhong on 2022/02/27.

#include "material.h"

Material::Material(const Texture& diffuse1,
                   const Texture& specular1,
                   float shininess)
    : diffuse1_(diffuse1),
      specular1_(specular1),
      shininess_(shininess) {}

void Material::SetShinieness(float shininess) {
  shininess_ = shininess;
}
