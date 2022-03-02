// Created by Dong Zhong on 2022/03/01.

#include "model.h"

Model::Model(const std::vector<Vertex>& vertices,
             const std::vector<GLuint>& indices)
    : vertices_(vertices),
      indices_(indices),
      model_trans_(glm::mat4(1.0f)) {
  Setup();
}

void Model::SetDiffuseTexture(const std::shared_ptr<Texture>& diffuse) {
  diffuse1_ = diffuse;
}

void Model::SetSpecularTexture(const std::shared_ptr<Texture>& specular) {
  specular1_ = specular;
}

void Model::SetModelTransformation(const glm::mat4& model_trans) {
  model_trans_ = model_trans;
}

void Model::Render(const std::shared_ptr<Material>& material,
                   const std::shared_ptr<GlobalController>& global_controller,
                   const std::shared_ptr<LightController>& light_controller) {
  auto shader = material->GetRenderShader();

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, diffuse1_->GetID());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, specular1_->GetID());

  shader->SetInt("material.diffuse1", 1);
  shader->SetInt("material.specular1", 2);
  shader->SetBool("material.is_blinn_phong", material->IsBlinnPhong());
  shader->SetFloat("material.shininess", (float)material->GetShininess());

  light_controller->ApplyLighting(shader, global_controller);

  Draw(shader);
}

void Model::Draw(const std::shared_ptr<Shader>& shader) {
  shader->SetMat4("model", model_trans_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Model::Setup() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));

  glBindVertexArray(0);
}
