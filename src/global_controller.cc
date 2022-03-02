// Created by Dong Zhong on 2022/03/01.

#include "global_controller.h"

#include <imgui.h>

GlobalController::GlobalController()
    : screen_size_(glm::vec2(1280, 720)),
      clear_color_(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)),
      gamma_enabled_(true),
      shadow_enabled_(true),
      displaying_shadow_map_(shadow_enabled_),
      is_drawing_coords_(true),
      camera_(std::make_shared<Camera>(glm::vec3(0.2f, 0.3f, 3.0f))) {
  coords_shader_ = std::make_shared<Shader>("vertex_shader.vs", "coords_fragment_shader.fs");
  GenerateCoordVAO();
}

void GlobalController::SetScreenSize(const glm::vec2& size) {
  screen_size_ = size;
}

void GlobalController::SetClearColor(const glm::vec4& clear_color) {
  clear_color_ = clear_color;
}

void GlobalController::SetGammaEnabled(bool enabled) {
  gamma_enabled_ = enabled;
}

void GlobalController::SetShadowEnabled(bool enabled) {
  shadow_enabled_ = enabled;
}

void GlobalController::SetDisplayingShadowMap(bool displaying) {
  displaying_shadow_map_ = displaying;
}

glm::mat4 GlobalController::GetViewMatrix() const {
  return camera_->GetViewMatrix();
}

void GlobalController::CameraMove(Camera::Direction direction, float delta_time) {
  camera_->Move(direction, delta_time);
}

void GlobalController::CameraRotate(Camera::Rotation rotation, float delta_time) {
  camera_->Rotate(rotation, delta_time);
}

void GlobalController::Config() {
  ImGui::PushID("Control");
  ImGui::Begin("Control");

  ImGui::ColorEdit4("clear color", (float*)&clear_color_);

  ImGui::Checkbox("Draw coordinates", &is_drawing_coords_);

  if (camera_) {
    ImGui::Text("Camera position: (%.3f, %.3f, %.3f)",
                camera_->GetPosition().x,
                camera_->GetPosition().y,
                camera_->GetPosition().z);
  }

  ImGui::Checkbox("Gamma", &gamma_enabled_);

  ImGui::Checkbox("Shadow", &shadow_enabled_);

  ImGui::Checkbox("Display shadow map", &displaying_shadow_map_);
  if (!shadow_enabled_) {
    displaying_shadow_map_ = false;
  }

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);

  ImGui::End();
  ImGui::PopID();
}

void GlobalController::RenderCoords() {
  if (!is_drawing_coords_) {
    return;
  }

  glBindVertexArray(coords_vao_);

  coords_shader_->Use();

  glm::mat4 model(1.0f);
  coords_shader_->SetMat4("model", model);

  coords_shader_->SetMat4("view", camera_->GetViewMatrix());
  coords_shader_->SetMat4("project", glm::perspective(glm::radians(45.0f),
                                                      (float)screen_size_.x / screen_size_.y,
                                                      0.1f, 100.0f));

  glDrawArrays(GL_LINES, 0, 6);

  glBindVertexArray(0);
}

void GlobalController::GenerateCoordVAO() {
  glGenVertexArrays(1, &coords_vao_);
  glBindVertexArray(coords_vao_);

  static GLfloat coord_vertices[] = {
    -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  };

  glGenBuffers(1, &coords_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, coords_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coord_vertices), coord_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}
