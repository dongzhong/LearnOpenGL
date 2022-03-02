// Created by Dong Zhong on 2022/02/27.

#ifndef LIGHT_H_
#define LIGHT_H_

#include <string>

#include <glm/glm.hpp>

class Light {
 public:
  enum class Type {
    kNone,
    kDirect,
    kPoint,
    kSpot,
  };

  Light(const glm::vec3& ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& diffuse = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& specular = glm::vec3(0.0f, 0.0f, 0.0f));

  virtual ~Light() = default;

  std::string GetName() const { return name_; }

  bool IsEnabled() const { return enable_; }
  void SetEnable(bool enable);

  glm::vec3 GetAmbient() const { return ambient_; }
  void SetAmbient(const glm::vec3& ambient);

  glm::vec3 GetDiffuse() const { return diffuse_; }
  void SetDiffuse(const glm::vec3& diffuse);

  glm::vec3 GetSpecular() const { return specular_; }
  void SetSpecular(const glm::vec3& specular);

  Type GetType() const { return type_; }

 protected:
  Type type_ = Type::kNone;

  std::string name_ = "Light";

  bool enable_;

  glm::vec3 ambient_;
  glm::vec3 diffuse_;
  glm::vec3 specular_;
};

class DirectLight : public Light {
 public:
  DirectLight(const glm::vec3& ambient = glm::vec3(0.05f, 0.05f, 0.05f),
              const glm::vec3& diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
              const glm::vec3& specular = glm::vec3(0.5f, 0.5, 0.5f),
              const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f));

  ~DirectLight() override = default;

  glm::vec3 GetDirection() const { return direction_; }
  void SetDirection(const glm::vec3& direction);

  glm::vec2 GetDirectionAngle() const;
  void SetDirectionAngle(float pitch, float yaw);

 private:
  glm::vec3 direction_;
};

class PointLight : public Light {
 public:
  PointLight(const glm::vec3& ambient = glm::vec3(0.05f, 0.05f, 0.05f),
             const glm::vec3& diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
             const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f),
             const glm::vec3& position = glm::vec3(0.0f, 0.0f, 1.0f),
             float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

  ~PointLight() override = default;

  glm::vec3 GetPosition() const { return position_; }
  void SetPosition(const glm::vec3& position);

  float GetConstant() const { return constant_; }
  void SetConstant(float constant);

  float GetLinear() const { return linear_; }
  void SetLinear(float linear);

  float GetQuadratic() const { return quadratic_; }
  void SetQuadratic(float quadratic);

 protected:
  glm::vec3 position_;

  float constant_;
  float linear_;
  float quadratic_;
};

class SpotLight : public PointLight {
 public:
  SpotLight(const glm::vec3& ambient = glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3& diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f),
            const glm::vec3& position = glm::vec3(0.0f, 0.0f, 1.0f),
            float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f,
            const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f),
            float cut_off = 12.5f, float outer_cut_off = 17.5f);

  ~SpotLight() override = default;

  glm::vec3 GetDirection() const { return direction_; }
  void SetDirection(const glm::vec3& direction);

  glm::vec2 GetDirectionAngle() const;
  void SetDirectionAngle(float pitch, float yaw);

  float GetCutOff() const { return cut_off_; }
  void SetCutOff(float cut_off);

  float GetOuterCutOff() const { return outer_cut_off_; }
  void SetOuterCutOff(float outer_cut_off);

 private:
  glm::vec3 direction_;

  float cut_off_;
  float outer_cut_off_;
};

#endif // LIGHT_H_
