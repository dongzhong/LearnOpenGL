// Created by Dong Zhong on 2022/03/01.

#ifndef SCENE_H_
#define SCENE_H_

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "global_controller.h"
#include "light_controller.h"
#include "material.h"
#include "model.h"
#include "vertex.h"

class Scene {
 public:
  Scene();
  ~Scene() = default;

  void AddMaterial(const std::string& name, const std::shared_ptr<Material>& material);

  void AddModel(const std::string& name, const std::shared_ptr<Model>& model, const std::string& material_name);

  void GenerateShadowMap(const std::shared_ptr<GlobalController>& global_controller,
                         const std::shared_ptr<LightController>& light_controller);

  void Render(const std::shared_ptr<GlobalController>& global_controller,
              const std::shared_ptr<LightController>& light_controller);

  void Config();

 private:
  static const GLuint kShadowWidth = 1024;
  static const GLuint kShadowHeight = 1024;

  void InitShadowMisc();
  void DisplayShadowMap();

  void ApplyAndSetShaderGlobal(const std::shared_ptr<Shader>& shader,
                               const std::shared_ptr<GlobalController>& global_controller);

  glm::mat4 light_space_trans_;

  GLuint shadow_fbo_;
  GLuint shadow_map_;
  std::shared_ptr<Shader> shadow_shader_;

  GLuint shadow_display_vao_;
  GLuint shadow_display_vbo_;
  std::shared_ptr<Shader> shadow_display_shader_;

  std::map<std::string, std::shared_ptr<Material>> materials_;

  std::map<std::string, std::pair<std::shared_ptr<Model>, std::string>> models_;
};

#endif // SCENE_H_
