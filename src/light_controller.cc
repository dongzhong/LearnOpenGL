// Created by Dong Zhong on 2022/03/01.

#include "light_controller.h"

#include <imgui.h>

LightController::LightController() = default;

void LightController::AddDirectLight(const std::string& name, const std::shared_ptr<DirectLight>& light) {
  direct_lights_[name] = light;
}

void LightController::AddPointLight(const std::string& name, const std::shared_ptr<PointLight>& light) {
  point_lights_[name] = light;
}

void LightController::AddFlashlight(std::shared_ptr<SpotLight> light) {
  flashlight_ = light;
}

void LightController::Config() {
  ImGui::PushID("Light");
  ImGui::Begin("Light");

  ImGui::PushItemWidth(200);

  // Direct Light
  for (auto&& [name, light] : direct_lights_) {
    if (light) {
      ImGui::PushID(name.c_str());

      ImGui::Text(name.c_str(), "");

      bool enable = light->IsEnabled();
      if (ImGui::Checkbox("Enable", &enable)) {
        light->SetEnable(enable);
      }

      glm::vec2 angles = light->GetDirectionAngle();
      bool angle_changed = ImGui::SliderFloat("Pitch", &angles.x, -90.0f, 90.0f);
      angle_changed = ImGui::SliderFloat("Yaw", &angles.y, 0.0f, 359.99f) || angle_changed;
      if (angle_changed) {
        light->SetDirectionAngle(angles.x, angles.y);
      }

      glm::vec3 color = light->GetAmbient();
      if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
        light->SetAmbient(color);
      }

      color = light->GetDiffuse();
      if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
        light->SetDiffuse(color);
      }

      color = light->GetSpecular();
      if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
        light->SetSpecular(color);
      }

      ImGui::Separator();
      ImGui::PopID();
    }
  }

  // Point Light
  for (auto&& [name, light] : point_lights_) {
    if (light) {
      ImGui::PushID(name.c_str());

      ImGui::Text(name.c_str(), "");

      bool enable = light->IsEnabled();
      if (ImGui::Checkbox("Enable", &enable)) {
        light->SetEnable(enable);
      }

      glm::vec3 point_position = light->GetPosition();
      if (ImGui::InputFloat3("Position", (float*)(&point_position))) {
        light->SetPosition(point_position);
      }

      auto color = light->GetAmbient();
      if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
        light->SetAmbient(color);
      }

      color = light->GetDiffuse();
      if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
        light->SetDiffuse(color);
      }

      color = light->GetSpecular();
      if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
        light->SetSpecular(color);
      }

      ImGui::Separator();
      ImGui::PopID();
    }
  }

  // Flashlight
  if (flashlight_) {
    ImGui::PushID("Flashlight");
    ImGui::Text("Flashlight");

    bool enable = (*flashlight_)->IsEnabled();
    if (ImGui::Checkbox("Enable", &enable)) {
      (*flashlight_)->SetEnable(enable);
    }

    auto color = (*flashlight_)->GetAmbient();
    if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
      (*flashlight_)->SetAmbient(color);
    }

    color = (*flashlight_)->GetDiffuse();
    if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
      (*flashlight_)->SetDiffuse(color);
    }

    color = (*flashlight_)->GetSpecular();
    if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
      (*flashlight_)->SetSpecular(color);
    }

    float cut_off = (*flashlight_)->GetCutOff();
    if (ImGui::SliderFloat("Cut-off", &cut_off, 0.0f, 90.0f)) {
      (*flashlight_)->SetCutOff(cut_off);
    }

    float outer_cut_off = (*flashlight_)->GetOuterCutOff();
    if (outer_cut_off < cut_off) {
      outer_cut_off = cut_off;
      (*flashlight_)->SetOuterCutOff(outer_cut_off);
    }
    if (ImGui::SliderFloat("Outer Cut-off", &outer_cut_off, cut_off, 90.0f)) {
      (*flashlight_)->SetOuterCutOff(outer_cut_off);
    }

    ImGui::Separator();
    ImGui::PopID();
  }

  ImGui::PopItemWidth();

  ImGui::End();
  ImGui::PopID();
}

void LightController::ApplyLighting(const std::shared_ptr<Shader>& shader,
                                    const std::shared_ptr<GlobalController>& global_controller) {
  std::size_t i = 0;
  for (auto&& [name, direct_light] : direct_lights_) {
    if (direct_light) {
      shader->SetBool("direct_light[" + std::to_string(i) + "].enable", direct_light->IsEnabled());
      shader->SetVec3("direct_light[" + std::to_string(i) + "].direction", direct_light->GetDirection());
      shader->SetVec3("direct_light[" + std::to_string(i) +"].ambient", direct_light->GetAmbient());
      shader->SetVec3("direct_light[" + std::to_string(i) + "].diffuse", direct_light->GetDiffuse());
      shader->SetVec3("direct_light[" + std::to_string(i) + "].specular", direct_light->GetSpecular());
    }
    ++i;
  }

  i = 0;
  for (auto&& [name, point_light] : point_lights_) {
    if (point_light) {
      shader->SetBool("point_light[" + std::to_string(i) + "].enable", point_light->IsEnabled());
      shader->SetVec3("point_light[" + std::to_string(i) + "].position", point_light->GetPosition());
      shader->SetVec3("point_light[" + std::to_string(i) + "].ambient", point_light->GetAmbient());
      shader->SetVec3("point_light[" + std::to_string(i) + "].diffuse", point_light->GetDiffuse());
      shader->SetVec3("point_light[" + std::to_string(i) + "].specular", point_light->GetSpecular());
      shader->SetFloat("point_light[" + std::to_string(i) + "].constant", point_light->GetConstant());
      shader->SetFloat("point_light[" + std::to_string(i) + "].linear", point_light->GetLinear());
      shader->SetFloat("point_light[" + std::to_string(i) + "].quadratic", point_light->GetQuadratic());
    }
    ++i;
  }

  if (flashlight_) {
    shader->SetBool("spot_light.enable", (*flashlight_)->IsEnabled());
    shader->SetVec3("spot_light.position", global_controller->GetCameraPosition());
    shader->SetVec3("spot_light.direction", global_controller->GetCameraFront());
    shader->SetFloat("spot_light.cut_off", glm::cos(glm::radians((*flashlight_)->GetCutOff())));
    shader->SetFloat("spot_light.outer_cut_off", glm::cos(glm::radians((*flashlight_)->GetOuterCutOff())));
    shader->SetVec3("spot_light.ambient", (*flashlight_)->GetAmbient());
    shader->SetVec3("spot_light.diffuse", (*flashlight_)->GetDiffuse());
    shader->SetVec3("spot_light.specular", (*flashlight_)->GetSpecular());
    shader->SetFloat("spot_light.constant", (*flashlight_)->GetConstant());
    shader->SetFloat("spot_light.linear", (*flashlight_)->GetLinear());
    shader->SetFloat("spot_light.quadratic", (*flashlight_)->GetQuadratic());
  }
}
