// Created by Dong Zhong on 2022/02/27.

#include "texture.h"

#include <iostream>

#include "stb_image.h"

Texture::Texture(GLuint texture_id) : texture_id_(texture_id) {}

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

