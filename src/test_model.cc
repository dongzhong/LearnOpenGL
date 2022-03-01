// Created by Dong Zhong on 2022/03/01.

#include "test_model.h"

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

Vertex vertex_array[] = {
  // front
  {pos_8, normal_1, tex_coords_1}, // 0
  {pos_5, normal_1, tex_coords_2}, // 1
  {pos_6, normal_1, tex_coords_4}, // 2
  {pos_7, normal_1, tex_coords_3}, // 3

  // back
  {pos_3, normal_2, tex_coords_1}, // 4
  {pos_2, normal_2, tex_coords_2}, // 5
  {pos_1, normal_2, tex_coords_4}, // 6
  {pos_4, normal_2, tex_coords_3}, // 7

  // up
  {pos_4, normal_3, tex_coords_1}, // 8
  {pos_8, normal_3, tex_coords_2}, // 9
  {pos_7, normal_3, tex_coords_4}, // 10
  {pos_3, normal_3, tex_coords_3}, // 11

  // bottom
  {pos_2, normal_4, tex_coords_1}, // 12
  {pos_6, normal_4, tex_coords_2}, // 13
  {pos_5, normal_4, tex_coords_4}, // 14
  {pos_1, normal_4, tex_coords_3}, // 15

  // right
  {pos_7, normal_5, tex_coords_1}, // 16
  {pos_6, normal_5, tex_coords_2}, // 17
  {pos_2, normal_5, tex_coords_4}, // 18
  {pos_3, normal_5, tex_coords_3}, // 19

  // left
  {pos_4, normal_6, tex_coords_1}, // 20
  {pos_1, normal_6, tex_coords_2}, // 21
  {pos_5, normal_6, tex_coords_4}, // 22
  {pos_8, normal_6, tex_coords_3}, // 23
};

GLuint index_array[] = {
  // front
  0, 1, 2, 0, 2, 3,
  // back
  4, 5, 6, 4, 6, 7,
  // up
  8, 9, 10, 8, 10, 11,
  // bottom
  12, 13, 14, 12, 14, 15,
  // right
  16, 17, 18, 16, 18, 19,
  // left
  20, 21, 22, 20, 22, 23,
};

glm::vec3 cube_position_array[] = {
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

Vertex plane_vertex_array[] = {
  {glm::vec3(10.0f, -0.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(10.0f, 0.0f)},
  {glm::vec3(-10.0f, -0.5f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
  {glm::vec3(-10.0f, -0.5f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 10.0f)},
  {glm::vec3(10.0f, -0.5f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(10.0f, 10.0f)}
};

GLuint plane_index_array[] = {
  0, 1, 2, 0, 2, 3,
};

std::vector<Vertex> TestModel::cube_vertices = std::vector<Vertex>(vertex_array, vertex_array + 24);

std::vector<GLuint> TestModel::cube_indices = std::vector<GLuint>(index_array, index_array + 36);

std::vector<glm::vec3> TestModel::cube_positions = std::vector<glm::vec3>(cube_position_array, cube_position_array + 10);

std::vector<Vertex> TestModel::plane_vertices = std::vector<Vertex>(plane_vertex_array, plane_vertex_array + 4);

std::vector<GLuint> TestModel::plane_indices = std::vector<GLuint>(plane_index_array, plane_index_array + 6);
