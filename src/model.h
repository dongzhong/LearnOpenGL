// Created by Dong Zhong on 2022/02/25.

#ifndef MODEL_H_
#define MODEL_H_

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model {
 public:
  Model(const char* path);

  void Draw(const Shader& shader);

 private:
  void LoadModel(const std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial* material,
                                            aiTextureType type,
                                            const std::string& type_name);

  std::vector<Mesh> meshes_;
  std::string directory_;
};

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

#endif // MODEL_H_
