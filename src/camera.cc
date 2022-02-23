// Created by Dong Zhong on 2022/02/23.

#include "camera.h"

const float kSpeed = 2.5f;
const float kSensitivity = 60.0f;

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : position_(position),
      world_up_(up),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      yaw_(yaw),
      pitch_(pitch) {
  UpdateCamera();
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::Move(Direction direction, float delta_time) {
  float velocity = kSpeed * delta_time;
  switch (direction) {
   case Direction::kForward:
    position_ += front_ * velocity;
    break;
   case Direction::kBackward:
    position_ -= front_ * velocity;
    break;
   case Direction::kRight:
    position_ += right_ * velocity;
    break;
   case Direction::kLeft:
    position_ -= right_ * velocity;
    break;
  }
}

void Camera::Rotate(Rotation rotation, float delta_time) {
  float offset = kSensitivity * delta_time;
  switch (rotation) {
   case Rotation::kUp:
    pitch_ += offset;
    break;
   case Rotation::kDown:
    pitch_ -= offset;
    break;
   case Rotation::kRight:
    yaw_ += offset;
    break;
   case Rotation::kLeft:
    yaw_ -= offset;
    break;
  }
  UpdateCamera();
}

void Camera::UpdateCamera() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front);
  right_ = glm::normalize(glm::cross(front_, world_up_));
  up_ = glm::normalize(glm::cross(right_, front_));
}
