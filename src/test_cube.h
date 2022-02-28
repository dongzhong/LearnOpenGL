// Created by Dong Zhong on 2022/02/28.

#ifndef TEST_CUBE_H_
#define TEST_CUBE_H_

#include <glad/glad.h>

#include "vertex.h"

class TestCube {
 public:
  static GLuint GenerateVAO();

  static Vertex vertices[42];

  static glm::vec3 cube_positions[10];
};

#endif // TEST_CUBE_H_
