// Created by Dong Zhong on 2022/02/18.

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

void Init();

void Draw();

int main() {
  // TODO:
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  Init();

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const char* kVertexShader = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec3 our_col;
out vec2 our_tex;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 project;

void main() {
  gl_Position = project * view * trans * vec4(pos.x, pos.y, pos.z, 1.0);
  our_tex = tex;
}
)__SHADER__";

const char* kFragmentShader = R"__SHADER__(
#version 330 core
out vec4 frag_color;

in vec2 our_tex;

uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main() {
  frag_color = mix(texture(our_texture1, our_tex), texture(our_texture2, our_tex), 0.2);
}
)__SHADER__";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLuint program;
GLuint vertex_array;

GLuint texture_1, texture_2;

void Init() {
  // Texture
  glGenTextures(1, &texture_1);
  glGenTextures(1, &texture_2);

  glBindTexture(GL_TEXTURE_2D, texture_1);

  stbi_set_flip_vertically_on_load(true);

  int width, height, nr_channels;
  unsigned char *data = stbi_load("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/container.jpg", &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "DongZhong: " << "Failed to load texture";
  }

  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, texture_2);

  data = stbi_load("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/awesomeface.png", &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "DongZhong: " << "Failed to load texture";
  }

  stbi_image_free(data);

  // Shader and Program
  GLint success;
  char info_log[512];

  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &kVertexShader, nullptr);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Vertex shader compile error: " << info_log;
  }

  GLuint fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragment_shader, 1, &kFragmentShader, nullptr);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Fragment shader compile error: " << info_log;
  }

  program = glCreateProgram();

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, info_log);
    std::cout << "DongZhong: " << "Program link error: " << info_log;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);


  // Vertex Buffer Object
  glGenVertexArrays(1, &vertex_array);

  glBindVertexArray(vertex_array);

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glUseProgram(program);

  glUniform1i(glGetUniformLocation(program, "our_texture1"), 0);
  glUniform1i(glGetUniformLocation(program, "our_texture2"), 1);
  std::cout << "DongZhong: " << glGetUniformLocation(program, "our_texture1") << ", " << glGetUniformLocation(program, "our_texture2");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_2);
}

void Draw() {
  glBindVertexArray(vertex_array);

  glm::mat4 trans(1.0f);
  trans = glm::translate(trans, glm::vec3(0.3f, 0.2f, 0.0f));
  trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  float time = (float)glfwGetTime();
  trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(program, "trans"), 1, GL_FALSE, glm::value_ptr(trans));

  glm::mat4 view(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 project = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_FALSE, glm::value_ptr(project));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}
