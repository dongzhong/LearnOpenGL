// Created by Dong Zhong on 2022/02/27.

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include <glad/glad.h>

#define TEXTURE_PATH "/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/textures"

class Texture {
 public:
  Texture(GLuint texture_id);

  GLuint GetID() const { return texture_id_; }

 private:
  GLuint texture_id_;
};

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

#endif // TEXTURE_H_
