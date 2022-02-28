// Created by Dong Zhong on 2022/02/28.

#include "test_cube.h"

const glm::vec3 pos_1(-0.5f, -0.5f, -0.5f);
const glm::vec3 pos_2(0.5f, -0.5f, -0.5f);
const glm::vec3 pos_3(0.5f, 0.5f, -0.5f);
const glm::vec3 pos_4(-0.5f, 0.5f, -0.5f);
const glm::vec3 pos_5(-0.5f, -0.5f, 0.5f);
const glm::vec3 pos_6(0.5f, -0.5f, 0.5f);
const glm::vec3 pos_7(0.5f, 0.5f, 0.5f);
const glm::vec3 pos_8(-0.5f, 0.5f, 0.5f);

const glm::vec3 normal_1(0.0f, 0.0f, 1.0f);
const glm::vec3 normal_2(0.0f, 0.0f, -1.0f);
const glm::vec3 normal_3(0.0f, 1.0f, 0.0f);
const glm::vec3 normal_4(0.0f, -1.0f, 0.0f);
const glm::vec3 normal_5(1.0f, 0.0f, 0.0f);
const glm::vec3 normal_6(-1.0f, 0.0f, 0.0f);

const glm::vec2 tex_coords_1(0.0f, 0.0f);
const glm::vec2 tex_coords_2(0.0f, 1.0f);
const glm::vec2 tex_coords_3(1.0f, 0.0f);
const glm::vec2 tex_coords_4(1.0f, 1.0f);

Vertex TestCube::vertices[36] = {
  // front
  {pos_8, normal_1, tex_coords_1},
  {pos_5, normal_1, tex_coords_2},
  {pos_6, normal_1, tex_coords_4},
  {pos_8, normal_1, tex_coords_1},
  {pos_6, normal_1, tex_coords_4},
  {pos_7, normal_1, tex_coords_3},

  // back
  {pos_3, normal_2, tex_coords_1},
  {pos_2, normal_2, tex_coords_2},
  {pos_1, normal_2, tex_coords_4},
  {pos_3, normal_2, tex_coords_1},
  {pos_1, normal_2, tex_coords_4},
  {pos_4, normal_2, tex_coords_3},

  // up
  {pos_4, normal_3, tex_coords_1},
  {pos_8, normal_3, tex_coords_2},
  {pos_7, normal_3, tex_coords_4},
  {pos_4, normal_3, tex_coords_1},
  {pos_7, normal_3, tex_coords_4},
  {pos_3, normal_3, tex_coords_3},

  // bottom
  {pos_2, normal_4, tex_coords_1},
  {pos_6, normal_4, tex_coords_2},
  {pos_5, normal_4, tex_coords_4},
  {pos_2, normal_4, tex_coords_1},
  {pos_5, normal_4, tex_coords_4},
  {pos_1, normal_4, tex_coords_3},

  // right
  {pos_7, normal_5, tex_coords_1},
  {pos_6, normal_5, tex_coords_2},
  {pos_2, normal_5, tex_coords_4},
  {pos_7, normal_5, tex_coords_1},
  {pos_2, normal_5, tex_coords_4},
  {pos_3, normal_5, tex_coords_3},

  // left
  {pos_4, normal_6, tex_coords_1},
  {pos_1, normal_6, tex_coords_2},
  {pos_5, normal_6, tex_coords_4},
  {pos_4, normal_6, tex_coords_1},
  {pos_5, normal_6, tex_coords_4},
  {pos_8, normal_6, tex_coords_3},
};

glm::vec3 TestCube::cube_positions[10] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLuint TestCube::GenerateVAO() {
  GLuint cube_vao;

  glGenVertexArrays(1, &cube_vao);
  glBindVertexArray(cube_vao);

  GLuint cube_vbo;
  glGenBuffers(1, &cube_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TestCube::vertices), TestCube::vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  return cube_vao;
}
