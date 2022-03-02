// Created by Dong Zhong on 2022/03/01.

#ifndef MODEL_H_
#define MODEL_H_

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "global_controller.h"
#include "light_controller.h"
#include "material.h"
#include "texture.h"
#include "vertex.h"

class Model {
 public:
  Model(const std::vector<Vertex>& vertices,
        const std::vector<GLuint>& indices);

  void SetDiffuseTexture(const std::shared_ptr<Texture>& diffuse);
  std::shared_ptr<Texture> GetDiffuseTexture() const { return diffuse1_; }

  void SetSpecularTexture(const std::shared_ptr<Texture>& specular);
  std::shared_ptr<Texture> GetSpecularTexture() const { return specular1_; }

  glm::mat4 GetModelTranformation() const { return model_trans_; }
  void SetModelTransformation(const glm::mat4& model_trans);

  void Render(const std::shared_ptr<Material>& material,
              const std::shared_ptr<GlobalController>& global_controller,
              const std::shared_ptr<LightController>& light_controller);

  void Draw(const std::shared_ptr<Shader>& shader);

 private:
  void Setup();

  GLuint vao_;
  GLuint vbo_;
  GLuint ebo_;

  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;

  std::shared_ptr<Texture> diffuse1_;
  std::shared_ptr<Texture> specular1_;

  glm::mat4 model_trans_;
};

#endif // MODEL_H_
