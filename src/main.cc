// Created by Dong Zhong on 2022/02/18.

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

int screen_width = 800;
int screen_height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float delta_time = 0.0f;
float current_time = 0.0f;

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

glm::vec3 cube_positions[] = {
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

glm::vec3 point_light[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main() {
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

  Shader shader("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/vertex_shader.vs",
                "/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/fragment_shader.fs");
  Shader light_shader("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/light_vertex_shader.vs",
                      "/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/light_fragment_shader.fs");

  GLuint vertex_array;
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

  GLuint light_array;
  glGenVertexArrays(1, &light_array);
  glBindVertexArray(light_array);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  GLuint texture_diffuse, texture_specular;
  glGenTextures(1, &texture_diffuse);
  glBindTexture(GL_TEXTURE_2D, texture_diffuse);

  stbi_set_flip_vertically_on_load(true);

  int width, height, nr_channels;
  unsigned char *data = stbi_load("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/container2.png",
                                  &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "DongZhong: " << "Failed to load texture";
  }

  stbi_image_free(data);

  glGenTextures(1, &texture_specular);
  glBindTexture(GL_TEXTURE_2D, texture_specular);

  data = stbi_load("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/container2_specular.png",
                    &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "DongZhong: " << "Failed to load texture";
  }

  stbi_image_free(data);

  current_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - current_time;
    current_time = current_frame;

    ProcessInput(window);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();
    glBindVertexArray(vertex_array);

    shader.SetInt("material.diffuse", 0);
    shader.SetInt("material.specular", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_specular);

    auto view = camera.GetViewMatrix();
    shader.SetMat4("view", view);

    glm::mat4 project = glm::perspective(glm::radians(45.0f), (float)screen_width / screen_height, 0.1f, 100.0f);
    shader.SetMat4("project", project);

    shader.SetVec3("view_pos", camera.GetPosition());

    shader.SetFloat("material.shininess", 32.0f);

    shader.SetVec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
    shader.SetVec3("dir_light.ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("dir_light.diffuse", 0.4f, 0.4f, 0.4f);
    shader.SetVec3("dir_light.specular", 0.5f, 0.5f, 0.5f);

    shader.SetVec3("point_light[0].position", point_light[0]);
    shader.SetVec3("point_light[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("point_light[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.SetVec3("point_light[0].specular", 1.0f, 1.0f, 1.0f);
    shader.SetFloat("point_light[0].constant", 1.0f);
    shader.SetFloat("point_light[0].linear", 0.09f);
    shader.SetFloat("point_light[0].quadratic", 0.032f);

    shader.SetVec3("point_light[1].position", point_light[1]);
    shader.SetVec3("point_light[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("point_light[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.SetVec3("point_light[1].specular", 1.0f, 1.0f, 1.0f);
    shader.SetFloat("point_light[1].constant", 1.0f);
    shader.SetFloat("point_light[1].linear", 0.09f);
    shader.SetFloat("point_light[1].quadratic", 0.032f);

    shader.SetVec3("point_light[2].position", point_light[2]);
    shader.SetVec3("point_light[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("point_light[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.SetVec3("point_light[2].specular", 1.0f, 1.0f, 1.0f);
    shader.SetFloat("point_light[2].constant", 1.0f);
    shader.SetFloat("point_light[2].linear", 0.09f);
    shader.SetFloat("point_light[2].quadratic", 0.032f);

    shader.SetVec3("point_light[3].position", point_light[3]);
    shader.SetVec3("point_light[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("point_light[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.SetVec3("point_light[3].specular", 1.0f, 1.0f, 1.0f);
    shader.SetFloat("point_light[3].constant", 1.0f);
    shader.SetFloat("point_light[3].linear", 0.09f);
    shader.SetFloat("point_light[3].quadratic", 0.032f);

    shader.SetVec3("spot_light.position", camera.GetPosition());
    shader.SetVec3("spot_light.direction", camera.GetFront());
    shader.SetFloat("spot_light.cut_off", glm::cos(glm::radians(12.5f)));
    shader.SetFloat("spot_light.outer_cut_off", glm::cos(glm::radians(17.5f)));
    shader.SetVec3("spot_light.ambient", 0.0f, 0.0f, 0.0f);
    shader.SetVec3("spot_light.diffuse", 1.0f, 1.0f, 1.0f);
    shader.SetVec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
    shader.SetFloat("spot_light.constant", 1.0f);
    shader.SetFloat("spot_light.linear", 0.09f);
    shader.SetFloat("spot_light.quadratic", 0.032f);

    for (int i = 0; i < 10; ++i) {
      glm::mat4 trans(1.0f);
      trans = glm::translate(trans, cube_positions[i]);
      trans = glm::rotate(trans, glm::radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetMat4("trans", trans);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    light_shader.Use();
    glBindVertexArray(light_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    light_shader.SetMat4("view", view);
    light_shader.SetMat4("project", project);

    for (int i = 0; i < 4; ++i) {
      glm::mat4 trans(1.0f);
      trans = glm::translate(trans, point_light[i]);
      trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
      light_shader.SetMat4("trans", trans);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  screen_width = width;
  screen_height = height;
}

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
