// Created by Dong Zhong on 2022/02/18.

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

void Init();

void Draw();

float delta_time = 0.0f;
float current_time = 0.0f;

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

  current_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - current_time;
    current_time = current_frame;

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

Camera camera;

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kForward, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kBackward, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kRight, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kLeft, delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    camera.Rotate(Camera::Rotation::kUp, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    camera.Rotate(Camera::Rotation::kDown, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    camera.Rotate(Camera::Rotation::kRight, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    camera.Rotate(Camera::Rotation::kLeft, delta_time);
  }
}

const char* kVertexShader = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 normal;

out vec3 our_col;
out vec2 our_tex;
out vec3 our_normal;
out vec3 frag_pos;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 project;

void main() {
  gl_Position = project * view * trans * vec4(pos, 1.0);
  our_tex = tex;
  our_normal = mat3(transpose(inverse(trans))) * normal;
  frag_pos = vec3(trans * vec4(pos, 1.0));
}
)__SHADER__";

const char* kFragmentShader = R"__SHADER__(
#version 330 core
out vec4 frag_color;

in vec2 our_tex;
in vec3 our_normal;
in vec3 frag_pos;

uniform vec3 light;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main() {
  vec4 object_color = mix(texture(our_texture1, our_tex), texture(our_texture2, our_tex), 0.2);

  float ambient_trength = 0.2;
  vec3 ambient = ambient_trength * light;

  vec3 norm = normalize(our_normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = diff * light;

  float specular_strength = 0.5;
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specular_strength * spec * light;

  vec3 light_col = ambient + diffuse + specular;
  frag_color = object_color * vec4(light_col, 1.0);
}
)__SHADER__";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glUseProgram(program);

  glUniform1i(glGetUniformLocation(program, "our_texture1"), 0);
  glUniform1i(glGetUniformLocation(program, "our_texture2"), 1);

  glm::vec3 light_color(1.0f, 1.0f, 1.0f);
  glUniform3fv(glGetUniformLocation(program, "light"), 1, glm::value_ptr(light_color));

  glm::vec3 light_position(1.0f, 2.0f, 0.0f);
  glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, glm::value_ptr(light_position));

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

  auto view = camera.GetViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 project = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_FALSE, glm::value_ptr(project));

  glUniform3fv(glGetUniformLocation(program, "view_pos"), 1, glm::value_ptr(camera.GetPosition()));

  glDrawArrays(GL_TRIANGLES, 0, 36);

  trans = glm::mat4(1.0);
  trans = glm::translate(trans, glm::vec3(-0.6f, -0.2f, -4.0f));
  trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  trans = glm::rotate(trans, -time * 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(program, "trans"), 1, GL_FALSE, glm::value_ptr(trans));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}
