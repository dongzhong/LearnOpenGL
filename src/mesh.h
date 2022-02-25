// Created by Dong Zhong on 2022/02/25.

#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Mesh;
class Shader;

class Vertex {
 public:
  Vertex(const glm::vec3& position,
         const glm::vec3& normal,
         const glm::vec2& texture_coords);

 private:
  glm::vec3 position_;
  glm::vec3 normal_;
  glm::vec2 texture_coords_;

  friend class Mesh;
};

class Texture {
 public:
  Texture(GLuint id, const std::string& type, const aiString& path);

 private:
  GLuint texture_id_;
  std::string type_;
  aiString path_;

  friend class Mesh;
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<GLuint>& indices,
       const std::vector<Texture>& textures);

  void Draw(const Shader& shader);

 private:
  void SetupMesh();

  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;

  GLuint vao_, vbo_, ebo_;
};

#endif // MESH_H_
