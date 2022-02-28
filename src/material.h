// Created by Dong Zhong on 2022/02/27.

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glad/glad.h>

#include "texture.h"

class Material {
 public:
  Material(const Texture& diffuse1,
           const Texture& specular1,
           float shininess);

  Texture GetDiffuse1() const { return diffuse1_; }

  Texture GetSpecular1() const { return specular1_; }

  float GetShininess() const { return shininess_; }
  void SetShinieness(float shininess);

 private:
  Texture diffuse1_;
  Texture specular1_;
  float shininess_;
};

#endif // MATERIAL_H_
