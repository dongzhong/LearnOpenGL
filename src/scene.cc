// Created by Dong Zhong on 2022/03/01.

#include "scene.h"

#include <imgui.h>

#include <iostream>

Scene::Scene() {
  InitShadowMisc();
}

void Scene::AddMaterial(const std::string& name, const std::shared_ptr<Material>& material) {
  materials_[name] = material;
}

void Scene::AddModel(const std::string& name, const std::shared_ptr<Model>& model, const std::string& material_name) {
  models_[name] = std::make_pair(model, material_name);
}

void Scene::GenerateShadowMap(const std::shared_ptr<GlobalController>& global_controller,
                              const std::shared_ptr<LightController>& light_controller) {
  // [Note] Assume first direct light, if exist.
  /*glViewport(0, 0, kShadowWidth, kShadowHeight);
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo_);
  glClear(GL_DEPTH_BUFFER_BIT);

  glCullFace(GL_FRONT);
  if (light_controller->GetDirectLights().size() > 0) {
    shadow_shader_->Use();

    GLfloat near_plane = 0.1f, far_plane = 10.0f;
    auto light_project = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    auto light_dir = light_controller->GetDirectLights().begin()->second->GetDirection();
    auto light_view = glm::lookAt(-light_dir * 5.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    light_space_trans_ = light_project * light_view;

    shadow_shader_->SetMat4("light_space_trans", light_space_trans_);

    for (auto&& [name, model_pair] : models_) {
      model_pair.first->Draw(shadow_shader_);
    }
  }
  glCullFace(GL_BACK);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

  for (auto&& [name, direct_light] : light_controller->GetDirectLights()) {
    glViewport(0, 0, Light::kShadowWidth, Light::kShadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, direct_light->GetShadowFBO());
    glClear(GL_DEPTH_BUFFER_BIT);

    shadow_shader_->Use();

    shadow_shader_->SetMat4("light_space_trans", direct_light->GetLightSpaceTrans());

    for (auto&& [name, model_pair] : models_) {
      model_pair.first->Draw(shadow_shader_);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Scene::Render(const std::shared_ptr<GlobalController>& global_controller,
                   const std::shared_ptr<LightController>& light_controller) {
  if (global_controller->IsShadowEnabled()) {
    GenerateShadowMap(global_controller, light_controller);
  }

  auto screen_size = global_controller->GetScreenSize();
  glViewport(0, 0, screen_size.x, screen_size.y);

  glEnable(GL_DEPTH_TEST);
  auto clear_color = global_controller->GetClearColor();
  glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  global_controller->RenderCoords();

  if (global_controller->IsShadowEnabled()) {
    for (auto&& [name, model_pair] : models_) {
      auto material = materials_[model_pair.second];
      auto shader = material->GetRenderShader();

      ApplyAndSetShaderGlobal(shader, global_controller);

      unsigned int i = 0;
      for (auto&& [name, direct_light] : light_controller->GetDirectLights()) {
        shader->SetMat4("light_space_trans", direct_light->GetLightSpaceTrans());

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, direct_light->GetShadowMap());
        shader->SetInt("shadow_map", i);
        shader->SetBool("shadow_enable", true);
        ++i;
      }

      model_pair.first->Render(material, global_controller, light_controller);
    }
  } else {
    for (auto&& [name, model_pair] : models_) {
      auto material = materials_[model_pair.second];
      auto shader = material->GetRenderShader();

      ApplyAndSetShaderGlobal(shader, global_controller);

      shader->SetBool("shadow_enable", false);

      model_pair.first->Render(material, global_controller, light_controller);
    }
  }

  if (global_controller->IsDisplayingShadowMap()) {
    DisplayShadowMap(light_controller);
  }
}

void Scene::Config() {
  ImGui::PushID("Materials");
  ImGui::Begin("Materials");

  std::size_t i = 0;
  for (auto&& [name, material] : materials_) {
    if (material) {
      ImGui::PushID(name.c_str());

      ImGui::Text(name.c_str(), "");

      bool is_blinn_phong = material->IsBlinnPhong();
      if (ImGui::Checkbox("Blinn-Phong", &is_blinn_phong)) {
        material->SetBlinnPhong(is_blinn_phong);
      }

      ImGui::Text("Shininess:");

      if (ImGui::RadioButton("2", material->GetShininess() == 2)) { material->SetShinieness(2); } ImGui::SameLine();
      if (ImGui::RadioButton("4", material->GetShininess() == 4)) { material->SetShinieness(4); } ImGui::SameLine();
      if (ImGui::RadioButton("8", material->GetShininess() == 8)) { material->SetShinieness(8); } ImGui::SameLine();
      if (ImGui::RadioButton("16", material->GetShininess() == 16)) { material->SetShinieness(16); }
      if (ImGui::RadioButton("32", material->GetShininess() == 32)) { material->SetShinieness(32); } ImGui::SameLine();
      if (ImGui::RadioButton("64", material->GetShininess() == 64)) { material->SetShinieness(64); } ImGui::SameLine();
      if (ImGui::RadioButton("128", material->GetShininess() == 128)) { material->SetShinieness(128); } ImGui::SameLine();
      if (ImGui::RadioButton("256", material->GetShininess() == 256)) { material->SetShinieness(256); }

      ImGui::Separator();

      ImGui::PopID();
    }
  }

  ImGui::End();
  ImGui::PopID();
}

void Scene::InitShadowMisc() {
  shadow_shader_ = std::make_shared<Shader>("shadow_shader.vs", "shadow_shader.fs");

  // Shadow display
  glGenVertexArrays(1, &shadow_display_vao_);
  glGenBuffers(1, &shadow_display_vbo_);

  glBindVertexArray(shadow_display_vao_);

  glBindBuffer(GL_ARRAY_BUFFER, shadow_display_vbo_);

  static float plane_vertices[] = {
    -0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.99f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.49f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.49f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.49f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(6 * sizeof(GLfloat)));

  glBindVertexArray(0);

  shadow_display_shader_ = std::make_shared<Shader>("vertex_shader.vs", "shadow_display.fs");
}

void Scene::DisplayShadowMap(const std::shared_ptr<LightController>& light_controller) {
  // Display shadow map
  /*shadow_display_shader_->Use();

  shadow_display_shader_->SetMat4("project", glm::mat4(1.0f));
  shadow_display_shader_->SetMat4("view", glm::mat4(1.0f));
  shadow_display_shader_->SetMat4("model", glm::mat4(1.0f));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadow_map_);
  shadow_display_shader_->SetInt("shadow_map", 0);

  glBindVertexArray(shadow_display_vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);*/

  /*ImGui::PushID("ShadowMap");
  ImGui::Begin("Shadow map");

  ImGui::Image((void*)(intptr_t)shadow_map_, ImVec2(200, 200));

  ImGui::End();
  ImGui::PopID();*/

  for (auto&& [name, direct_light] : light_controller->GetDirectLights()) {
    ImGui::PushID(name.c_str());
    ImGui::Begin(name.c_str());

    ImGui::Image((void*)(intptr_t)(direct_light->GetShadowMap()), ImVec2(200, 200));

    ImGui::End();
    ImGui::PopID();
  }
}

void Scene::ApplyAndSetShaderGlobal(const std::shared_ptr<Shader>& shader,
                                    const std::shared_ptr<GlobalController>& global_controller) {
  shader->Use();

  shader->SetBool("gamma", global_controller->IsGammaEnabled());
  shader->SetMat4("view", global_controller->GetViewMatrix());
  auto screen_size = global_controller->GetScreenSize();
  shader->SetMat4("project", glm::perspective(glm::radians(45.0f), (float)screen_size.x / screen_size.y, 0.1f, 100.0f));
  shader->SetVec3("view_position", global_controller->GetCameraPosition());
}
