// Created by Dong Zhong on 2022/02/25.

#include "mesh.h"

#include <iostream>

#include "shader.h"

Vertex::Vertex(const glm::vec3& position,
               const glm::vec3& normal,
               const glm::vec2& texture_coords)
    : position_(position),
      normal_(normal),
      texture_coords_(texture_coords) {}

Texture::Texture(GLuint id, const std::string& type, const aiString& path)
    : texture_id_(id), type_(type), path_(path) {}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const std::vector<Texture>& textures)
    : vertices_(vertices),
      indices_(indices),
      textures_(textures) {
  SetupMesh();
}

void Mesh::Draw(const Shader& shader) {
  unsigned int diffuse_num = 1;
  unsigned int specular_num = 1;
  for (unsigned int i = 0; i < textures_.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures_[i].type_;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuse_num++);
    } else if (name == "texture_specular") {
      number = std::to_string(specular_num++);
    }
    shader.SetInt(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures_[i].texture_id_);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::SetupMesh() {
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords_));

  glBindVertexArray(0);
}
