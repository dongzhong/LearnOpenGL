// Created by Dong Zhong on 2022/03/01.

#ifndef RENDER_CONTROLLER_H_
#define RENDER_CONTROLLER_H_

#include <map>
#include <string>

#include "global_controller.h"
#include "light.h"
#include "shader.h"

class LightController {
 public:
  LightController();

  void AddDirectLight(const std::string& name, const std::shared_ptr<DirectLight>& light);

  const std::map<std::string, std::shared_ptr<DirectLight>>& GetDirectLights() const { return direct_lights_; }

  void AddPointLight(const std::string& name, const std::shared_ptr<PointLight>& light);

  const std::map<std::string, std::shared_ptr<PointLight>>& GetPointLights() const { return point_lights_; }

  void AddSpotLight(const std::string& name, const std::shared_ptr<SpotLight>& spot_light);

  const std::map<std::string, std::shared_ptr<SpotLight>>& GetSpotLight() const { return spot_lights_; }

  void AddFlashlight(std::shared_ptr<SpotLight> light);

  std::shared_ptr<SpotLight> GetFlashlight() const { return *flashlight_; }

  void Config();

  void ApplyLighting(const std::shared_ptr<Shader>& shader,
                     const std::shared_ptr<GlobalController>& global_controller);

 private:
  std::map<std::string, std::shared_ptr<DirectLight>> direct_lights_;
  std::map<std::string, std::shared_ptr<PointLight>> point_lights_;
  std::map<std::string, std::shared_ptr<SpotLight>> spot_lights_;
  std::optional<std::shared_ptr<SpotLight>> flashlight_;

  glm::vec3 clear_color_ {0.1f, 0.1f, 0.1f};
};

#endif // RENDER_CONTROLLER_H_
