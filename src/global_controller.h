// Created by Dong Zhong on 2022/03/01.

#ifndef GLOBAL_CONTROLLER_H_
#define GLOBAL_CONTROLLER_H_

#include <memory>

#include <glm/glm.hpp>

#include "camera.h"
#include "shader.h"

class GlobalController {
 public:
  GlobalController();
  ~GlobalController() = default;

  glm::vec2 GetScreenSize() const { return screen_size_; }
  void SetScreenSize(const glm::vec2& size);

  glm::vec4 GetClearColor() const { return clear_color_; }
  void SetClearColor(const glm::vec4& clear_color);

  bool IsGammaEnabled() const { return gamma_enabled_; }
  void SetGammaEnabled(bool enabled);

  glm::mat4 GetViewMatrix() const;

  void CameraMove(Camera::Direction direction, float delta_time);

  void CameraRotate(Camera::Rotation rotation, float delta_time);

  glm::vec3 GetCameraPosition() const { return camera_->GetPosition(); }

  glm::vec3 GetCameraFront() const { return camera_->GetFront(); }

  void Config();

  void RenderCoords();

 private:
  void GenerateCoordVAO();

  glm::vec2 screen_size_;

  glm::vec4 clear_color_;
  bool gamma_enabled_;

  bool is_drawing_coords_;
  std::shared_ptr<Shader> coords_shader_;
  GLuint coords_vao_;
  GLuint coords_vbo_;

  std::shared_ptr<Camera> camera_;
};

#endif // GLOBAL_CONTROLLER_H_
