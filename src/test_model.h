// Created by Dong Zhong on 2022/03/01.

#ifndef TEST_MODEL_H_
#define TEST_MODEL_H_

#include <vector>

#include <glad/glad.h>

#include "vertex.h"

class TestModel {
 public:
  static std::vector<Vertex> cube_vertices;
  static std::vector<GLuint> cube_indices;
  static std::vector<glm::vec3> cube_positions;

  static std::vector<Vertex> plane_vertices;
  static std::vector<GLuint> plane_indices;
};

#endif // TEST_MODEL_H_
