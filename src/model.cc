// Created by Dong Zhong on 2022/02/25.

#include "model.h"

#include <iostream>

#include <assimp/Importer.hpp>

#include "stb_image.h"

Model::Model(const char* path) {
  LoadModel(path);
}

void Model::Draw(const Shader& shader) {
  for (auto mesh : meshes_) {
    mesh.Draw(shader);
  }
}

void Model::LoadModel(const std::string& path) {
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  directory_ = path.substr(0, path.find_last_of('/'));

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    glm::vec3 position(mesh->mVertices[i].x,mesh->mVertices[i].y, mesh->mVertices[i].z);

    glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

    glm::vec2 tex_coords(0.0f, 0.0f);
    if (mesh->mTextureCoords[0]) {
      tex_coords.x = mesh->mTextureCoords[0][i].x;
      tex_coords.y = mesh->mTextureCoords[0][i].y;
    }

    Vertex vertex(position, normal, tex_coords);

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    auto face = mesh->mFaces[i];
    for (unsigned int k = 0; k < face.mNumIndices; ++k) {
      indices.push_back(face.mIndices[k]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    auto* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    auto specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }

  return { vertices, indices, textures };
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material,
                                                 aiTextureType type,
                                                 const std::string& type_name) {
  std::vector<Texture> textures_;
  for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
    aiString str;
    material->GetTexture(type, i, &str);
    Texture texture(TextureFromFile(str.C_Str(), directory_), type_name, str);
    textures_.push_back(texture);
  }
  return textures_;
}

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma) {
  std::string file_name = std::string(path);
  file_name = directory + '/' + file_name;

  GLuint texture;
  glGenTextures(1, &texture);

  int width, height, nr_channels;
  unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &nr_channels, 0);

  if (data) {
    GLenum format;
    if (nr_channels == 1) {
      format = GL_RED;
    } else if (nr_channels == 3) {
      format = GL_RGB;
    } else if (nr_channels == 4) {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    std::cout << "DongZhong: " << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  return texture;
}
