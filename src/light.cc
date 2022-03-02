// Created by Dong Zhong on 2022/02/27.

#include "light.h"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

Light::Light(const glm::vec3& ambient,
             const glm::vec3& diffuse,
             const glm::vec3& specular)
    : enable_(false),
      ambient_(ambient),
      diffuse_(diffuse),
      specular_(specular) {}

void Light::SetEnable(bool enable) {
  enable_ = enable;
}

void Light::SetAmbient(const glm::vec3& ambient) {
  ambient_ = ambient;
}

void Light::SetDiffuse(const glm::vec3& diffuse) {
  diffuse_ = diffuse;
}

void Light::SetSpecular(const glm::vec3& specular) {
  specular_ = specular;
}

DirectLight::DirectLight(const glm::vec3& ambient,
                         const glm::vec3& diffuse,
                         const glm::vec3& specular,
                         const glm::vec3& direction)
    : Light(ambient, diffuse, specular),
      direction_(glm::normalize(direction)) {
  type_ = Light::Type::kDirect;
  name_ = "Direct Light";
  enable_ = true;
}

void DirectLight::SetDirection(const glm::vec3& direction) {
  direction_ = glm::normalize(direction);
}

glm::vec2 DirectLight::GetDirectionAngle() const {
  float pitch, yaw;
  pitch = asin(direction_.y);
  if (direction_.x == 0.0f) {
    yaw = direction_.z > 0 ? 1.5f * M_PI : 0.5f * M_PI;
  } else {
    yaw = atan(-direction_.z / direction_.x);
  }

  float yaw_degree = glm::degrees(yaw);

  if (direction_.x < 0) {
    yaw_degree += 180.0f;
  } else if (direction_.z > 0) {
    yaw_degree += 360.0f;
  }

  return glm::vec2(glm::degrees(pitch), yaw_degree);
}

void DirectLight::SetDirectionAngle(float pitch, float yaw) {
  glm::vec3 direction;
  direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  direction.y = sin(glm::radians(pitch));
  direction.z = -cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  direction_ = glm::normalize(direction);
}

PointLight::PointLight(const glm::vec3& ambient,
                       const glm::vec3& diffuse,
                       const glm::vec3& specular,
                       const glm::vec3& position,
                       float constant, float linear, float quadratic)
    : Light(ambient, diffuse, specular),
      position_(position),
      constant_(constant), linear_(linear), quadratic_(quadratic) {
  type_ = Light::Type::kPoint;
  name_ = "Point Light";
}

void PointLight::SetPosition(const glm::vec3& position) {
  position_ = position;
}

void PointLight::SetConstant(float constant) {
  constant_ = constant;
}

void PointLight::SetLinear(float linear) {
  linear_ = linear;
}

void PointLight::SetQuadratic(float quadratic) {
  quadratic_ = quadratic;
}

SpotLight::SpotLight(const glm::vec3& ambient,
                     const glm::vec3& diffuse,
                     const glm::vec3& specular,
                     const glm::vec3& position,
                     float constant, float linear, float quadratic,
                     const glm::vec3& direction,
                     float cut_off, float outer_cut_off)
    : PointLight(ambient, diffuse, specular, position,
                 constant, linear, quadratic),
      direction_(glm::normalize(direction)),
      cut_off_(cut_off), outer_cut_off_(outer_cut_off) {
  type_ = Light::Type::kSpot;
  name_ = "Spot Light";
}

void SpotLight::SetDirection(const glm::vec3& direction) {
  direction_ = direction;
}

glm::vec2 SpotLight::GetDirectionAngle() const {
  float pitch, yaw;
  pitch = asin(direction_.y);
  if (direction_.x == 0.0f) {
    yaw = direction_.z > 0 ? 1.5f * M_PI : 0.5f * M_PI;
  } else {
    yaw = atan(-direction_.z / direction_.x);
  }

  float yaw_degree = glm::degrees(yaw);

  if (direction_.x < 0) {
    yaw_degree += 180.0f;
  } else if (direction_.z > 0) {
    yaw_degree += 360.0f;
  }

  return glm::vec2(glm::degrees(pitch), yaw_degree);
}

void SpotLight::SetDirectionAngle(float pitch, float yaw) {
  glm::vec3 direction;
  direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  direction.y = sin(glm::radians(pitch));
  direction.z = -cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  direction_ = glm::normalize(direction);
}

void SpotLight::SetCutOff(float cut_off) {
  cut_off_ = cut_off;
}

void SpotLight::SetOuterCutOff(float outer_cut_off) {
  outer_cut_off_ = outer_cut_off;
}
