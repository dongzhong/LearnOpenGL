// Created by Dong Zhong on 2022/02/27.

#include "vertex.h"

Vertex::Vertex(const glm::vec3& pos,
               const glm::vec3& normal,
               const glm::vec2& tex_coords)
    : position_(pos),
      normal_(normal),
      tex_coords_(tex_coords) {}
