// Created by Dong Zhong on 2022/02/18.

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "stb_image.h"

#include "global_controller.h"
#include "light_controller.h"
#include "light.h"
#include "material.h"
#include "scene.h"
#include "texture.h"
#include "vertex.h"

#include "test_model.h"

unsigned int g_width = 1280;
unsigned int g_height = 720;

std::shared_ptr<GlobalController> g_global_controller_;
std::shared_ptr<LightController> g_light_controller_;
std::shared_ptr<Scene> g_scene;

glm::vec3 point_light_position[] = {
  glm::vec3( 0.7f,  0.2f,  2.0f),
  glm::vec3( 2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -12.0f),
  glm::vec3( 0.0f,  0.0f, -3.0f)
};

bool is_drawing_coords = true;

float delta_time = 0.0f;
float current_time = 0.0f;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  g_global_controller_ = std::make_shared<GlobalController>();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  g_global_controller_->SetScreenSize(glm::vec2(display_w, display_h));

  g_light_controller_ = std::make_shared<LightController>();
  g_light_controller_->AddDirectLight("Direct Light", std::make_shared<DirectLight>());
  for (unsigned int i = 0; i < sizeof(point_light_position) / sizeof(glm::vec3); ++i) {
    auto point_light = std::make_shared<PointLight>();
    point_light->SetPosition(point_light_position[i]);
    g_light_controller_->AddPointLight("Point Light " + std::to_string(i), point_light);
  }
  g_light_controller_->AddSpotLight("Spot Light", std::make_shared<SpotLight>());
  g_light_controller_->AddFlashlight(std::make_shared<SpotLight>());

  g_scene = std::make_shared<Scene>();
  auto material = std::make_shared<Material>(32);
  auto shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs");
  material->SetRenderShader(shader);
  g_scene->AddMaterial("Cube", material);

  auto diffuse_texture = std::make_shared<Texture>(TextureFromFile("container.png", TEXTURE_PATH, true));
  auto specular_texture = std::make_shared<Texture>(TextureFromFile("container.png", TEXTURE_PATH, true));

  // Test Model
  for (std::size_t i = 0; i < TestModel::cube_positions.size(); ++i) {
    auto cube_model = std::make_shared<Model>(TestModel::cube_vertices, TestModel::cube_indices);
    cube_model->SetDiffuseTexture(diffuse_texture);
    cube_model->SetSpecularTexture(specular_texture);
    glm::mat4 cube_transform = glm::mat4(1.0);
    cube_transform = glm::translate(cube_transform, TestModel::cube_positions[i]);
    cube_transform = glm::rotate(cube_transform, glm::radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
    cube_model->SetModelTransformation(cube_transform);
    g_scene->AddModel("TestModel" + std::to_string(i), cube_model, "Cube");
  }

  auto plane_model = std::make_shared<Model>(TestModel::plane_vertices, TestModel::plane_indices);
  plane_model->SetDiffuseTexture(diffuse_texture);
  plane_model->SetSpecularTexture(specular_texture);
  g_scene->AddModel("TestPlane", plane_model, "Cube");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ProcessInput(window);

    float current_frame = glfwGetTime();
    delta_time = current_frame - current_time;
    current_time = current_frame;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    g_global_controller_->Config();
    g_light_controller_->Config();
    g_scene->Config();

    g_scene->Render(g_global_controller_, g_light_controller_);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  g_global_controller_->SetScreenSize(glm::vec2((float)width, (float)height));
}

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kForward, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kBackward, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kRight, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kLeft, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kUp, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    g_global_controller_->CameraMove(Camera::Direction::kDown, delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    g_global_controller_->CameraRotate(Camera::Rotation::kUp, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    g_global_controller_->CameraRotate(Camera::Rotation::kDown, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    g_global_controller_->CameraRotate(Camera::Rotation::kRight, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    g_global_controller_->CameraRotate(Camera::Rotation::kLeft, delta_time);
  }
}
