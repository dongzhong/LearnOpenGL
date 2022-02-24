// Created by Dong Zhong on 2022/02/23.

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
 public:
  enum class Direction {
    kForward,
    kBackward,
    kRight,
    kLeft,
  };

  enum class Rotation {
    kUp,
    kDown,
    kRight,
    kLeft,
  };

  Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = -90.0f,
         float pitch = 0.0f);

  glm::mat4 GetViewMatrix() const;

  void Move(Direction direction, float delta_time);

  void Rotate(Rotation rotation, float delta_time);

  glm::vec3 GetPosition() const { return position_; }

 private:
  void UpdateCamera();

  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  float yaw_;
  float pitch_;
};

#endif // CAMERA_H
