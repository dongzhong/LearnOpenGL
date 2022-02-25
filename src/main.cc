// Created by Dong Zhong on 2022/02/18.

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

int screen_width = 800;
int screen_height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float delta_time = 0.0f;
float current_time = 0.0f;

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

  Shader shader("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/model_vertex_shader.vs",
                "/Users/bilibili/DongZhong/myCodes/LearnOpenGL/src/model_fragment_shader.fs");

  stbi_set_flip_vertically_on_load(true);

  Model model("/Users/bilibili/DongZhong/myCodes/LearnOpenGL/res/model/nanosuit/nanosuit.obj");

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

    shader.SetVec3("view_pos", camera.GetPosition());

    shader.SetVec3("direct_light.direction", -0.2f, -1.0f, -0.3f);
    shader.SetVec3("direct_light.ambient", 0.05f, 0.05f, 0.05f);
    shader.SetVec3("direct_light.diffuse", 0.4f, 0.4f, 0.4f);
    shader.SetVec3("direct_light.specular", 0.5f, 0.5f, 0.5f);

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

    glm::mat4 model_trans(1.0f);
    model_trans = glm::translate(model_trans, glm::vec3(0.0f, -1.0f, 0.0f));
    model_trans = glm::scale(model_trans, glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetMat4("model", model_trans);

    auto view = camera.GetViewMatrix();
    shader.SetMat4("view", view);

    glm::mat4 project = glm::perspective(glm::radians(45.0f), (float)screen_width / screen_height, 0.1f, 100.0f);
    shader.SetMat4("project", project);

    model.Draw(shader);

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
