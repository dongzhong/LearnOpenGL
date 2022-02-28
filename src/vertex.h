// Created by Dong Zhong on 2022/02/27.

#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/glm.hpp>

class Vertex {
 public:
  Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& tex_coords);

  glm::vec3 GetPosition() const { return position_; }

  glm::vec3 GetNormal() const { return normal_; }

  glm::vec2 GetTexCoords() const { return tex_coords_; }

 private:
  glm::vec3 position_;
  glm::vec3 normal_;
  glm::vec2 tex_coords_;
};

#endif // VERTEX_H_
