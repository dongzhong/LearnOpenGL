// Created by Dong Zhong on 2022/03/01.

#include "scene.h"

#include <imgui.h>

Scene::Scene() = default;

void Scene::AddMaterial(const std::string& name, const std::shared_ptr<Material>& material) {
  materials_[name] = material;
}

void Scene::AddModel(const std::string& name, const std::shared_ptr<Model>& model, const std::string& material_name) {
  models_[name] = std::make_pair(model, material_name);
}

void Scene::Render(const std::shared_ptr<GlobalController>& global_controller,
                   const std::shared_ptr<LightController>& light_controller) {
  for (auto&& [name, model_pair] : models_) {
    model_pair.first->Render(materials_[model_pair.second], global_controller, light_controller);
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
