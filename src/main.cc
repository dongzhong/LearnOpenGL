// Created by Dong Zhong on 2022/02/18.

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "stb_image.h"

#include "camera.h"
#include "light.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include "vertex.h"

#include "test_cube.h"

unsigned int g_width = 1280;
unsigned int g_height = 720;

bool g_is_blinn_phong = false;

bool g_gamma = true;

Camera camera(glm::vec3(0.2f, 0.3f, 3.0f));

DirectLight direct_light;
PointLight point_light[4];
SpotLight spot_light;

glm::vec3 point_light_position[] = {
  glm::vec3( 0.7f,  0.2f,  2.0f),
  glm::vec3( 2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -12.0f),
  glm::vec3( 0.0f,  0.0f, -3.0f)
};

ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

bool is_drawing_coords = true;

float delta_time = 0.0f;
float current_time = 0.0f;

void RenderGUI();

void ProcessInput(GLFWwindow* window);

void DrawCoordinates(bool draw, const Shader& shader, GLuint vao);
GLuint GenerateCoordVAO();

void SettingMaterial(Material* material);
void SettingLight(DirectLight* light,
                  PointLight* point_lights,
                  std::size_t point_light_num,
                  SpotLight* spot_light);

void DrawScene(const Material& material, const Shader& shader, GLuint vao);

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

  Shader coords_shader("vertex_shader.vs", "coords_fragment_shader.fs");
  GLuint coords_vao = GenerateCoordVAO();

  Shader shader("vertex_shader.vs", "phong_lighting_model.fs");

  Texture texture_diffuse1(TextureFromFile("container.png", TEXTURE_PATH, true));
  Texture texture_specular1(TextureFromFile("container.png", TEXTURE_PATH, true));

  Material material(texture_diffuse1, texture_specular1, 32);

  for (std::size_t i = 0; i < sizeof(point_light) / sizeof(PointLight); ++i) {
    point_light[i].SetPosition(point_light_position[i]);
  }

  // TestCube
  GLuint cube_vao = TestCube::GenerateVAO();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ProcessInput(window);

    float current_frame = glfwGetTime();
    delta_time = current_frame - current_time;
    current_time = current_frame;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderGUI();

    SettingMaterial(&material);
    SettingLight(&direct_light, point_light, sizeof(point_light) / sizeof(PointLight), &spot_light);

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glEnable(GL_DEPTH_TEST);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawCoordinates(is_drawing_coords, coords_shader, coords_vao);

    DrawScene(material, shader, cube_vao);

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

void RenderGUI() {
  ImGui::Begin("Control");

  ImGui::ColorEdit3("clear color", (float*)&clear_color);

  ImGui::Checkbox("Draw coordinates", &is_drawing_coords);

  ImGui::Text("Camera position: (%.3f, %.3f, %.3f)",
              camera.GetPosition().x,
              camera.GetPosition().y,
              camera.GetPosition().z);

  ImGui::Checkbox("Blinn-Phong", &g_is_blinn_phong);

  ImGui::Checkbox("Gamma", &g_gamma);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);

  ImGui::End();
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
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kUp, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    camera.Move(Camera::Direction::kDown, delta_time);
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

void DrawCoordinates(bool draw, const Shader& shader, GLuint vao) {
  if (!draw) {
    return;
  }

  glBindVertexArray(vao);

  shader.Use();

  glm::mat4 model(1.0f);
  shader.SetMat4("model", model);

  shader.SetMat4("view", camera.GetViewMatrix());
  shader.SetMat4("project", glm::perspective(glm::radians(45.0f), (float)g_width / g_height, 0.1f, 100.0f));

  glDrawArrays(GL_LINES, 0, 6);

  glBindVertexArray(0);
}

GLuint GenerateCoordVAO() {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static GLfloat coord_vertices[] = {
    -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  };

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coord_vertices), coord_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  return vao;
}

void SettingMaterial(Material* material) {
  ImGui::Begin("Material");

  ImGui::Text("Shininess:");

  if (ImGui::RadioButton("2", material->GetShininess() == 2)) { material->SetShinieness(2); } ImGui::SameLine();
  if (ImGui::RadioButton("4", material->GetShininess() == 4)) { material->SetShinieness(4); } ImGui::SameLine();
  if (ImGui::RadioButton("8", material->GetShininess() == 8)) { material->SetShinieness(8); } ImGui::SameLine();
  if (ImGui::RadioButton("16", material->GetShininess() == 16)) { material->SetShinieness(16); }
  if (ImGui::RadioButton("32", material->GetShininess() == 32)) { material->SetShinieness(32); } ImGui::SameLine();
  if (ImGui::RadioButton("64", material->GetShininess() == 64)) { material->SetShinieness(64); } ImGui::SameLine();
  if (ImGui::RadioButton("128", material->GetShininess() == 128)) { material->SetShinieness(128); } ImGui::SameLine();
  if (ImGui::RadioButton("256", material->GetShininess() == 256)) { material->SetShinieness(256); }

  ImGui::End();
}

void SettingLight(DirectLight* direct_light,
                  PointLight* point_lights,
                  std::size_t point_light_num,
                  SpotLight* spot_light) {
  ImGui::Begin("Light");

  // Direct light
  ImGui::PushID("Direct Light");
  ImGui::Text("Direct Light");

  bool enable = direct_light->IsEnabled();
  if (ImGui::Checkbox("Enable", &enable)) {
    direct_light->SetEnable(enable);
  }

  glm::vec2 angles = direct_light->GetDirectionAngle();
  bool angle_changed = ImGui::SliderFloat("Pitch", &angles.x, -90.0f, 90.0f);
  angle_changed = ImGui::SliderFloat("Yaw", &angles.y, 0.0f, 359.99f) || angle_changed;
  if (angle_changed) {
    direct_light->SetDirectionAngle(angles.x, angles.y);
  }

  glm::vec3 color = direct_light->GetAmbient();
  if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
    direct_light->SetAmbient(color);
  }

  color = direct_light->GetDiffuse();
  if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
    direct_light->SetDiffuse(color);
  }

  color = direct_light->GetSpecular();
  if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
    direct_light->SetSpecular(color);
  }
  ImGui::PopID();

  ImGui::Separator();

  // Point light
  for (std::size_t i = 0; i < point_light_num; ++i) {
    std::string point_light_id = std::string("Point Light " + std::to_string(i));

    ImGui::PushID(point_light_id.c_str());
    ImGui::Text(point_light_id.c_str());

    enable = point_lights[i].IsEnabled();
    if (ImGui::Checkbox("Enable", &enable)) {
      point_lights[i].SetEnable(enable);
    }

    glm::vec3 point_position = point_lights[i].GetPosition();
    if (ImGui::InputFloat3("Position", (float*)(&point_position))) {
      point_lights[i].SetPosition(point_position);
    }

    color = point_lights[i].GetAmbient();
    if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
      point_lights[i].SetAmbient(color);
    }

    color = point_lights[i].GetDiffuse();
    if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
      point_lights[i].SetDiffuse(color);
    }

    color = point_lights[i].GetSpecular();
    if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
      point_lights[i].SetSpecular(color);
    }
    ImGui::PopID();

    ImGui::Separator();
  }

  // Spot light
  ImGui::PushID("Spot Light");
  ImGui::Text("Spot Light");

  enable = spot_light->IsEnabled();
  if (ImGui::Checkbox("Enable", &enable)) {
    spot_light->SetEnable(enable);
  }

  color = spot_light->GetAmbient();
  if (ImGui::ColorEdit3("Ambient Color", (float*)&color)) {
    spot_light->SetAmbient(color);
  }

  color = spot_light->GetDiffuse();
  if (ImGui::ColorEdit3("Diffuse Color", (float*)&color)) {
    spot_light->SetDiffuse(color);
  }

  color = spot_light->GetSpecular();
  if (ImGui::ColorEdit3("Specular Color", (float*)&color)) {
    spot_light->SetSpecular(color);
  }

  float cut_off = spot_light->GetCutOff();
  if (ImGui::SliderFloat("Cut-off", &cut_off, 0.0f, 90.0f)) {
    spot_light->SetCutOff(cut_off);
  }

  float outer_cut_off = spot_light->GetOuterCutOff();
  if (outer_cut_off < cut_off) {
    outer_cut_off = cut_off;
    spot_light->SetOuterCutOff(outer_cut_off);
  }
  if (ImGui::SliderFloat("Outer Cut-off", &outer_cut_off, cut_off, 90.0f)) {
    spot_light->SetOuterCutOff(outer_cut_off);
  }
  ImGui::PopID();

  ImGui::Separator();

  ImGui::End();
}

void DrawScene(const Material& material, const Shader& shader, GLuint vao) {
  glBindVertexArray(vao);

  shader.Use();

  shader.SetBool("is_blinn_phong", g_is_blinn_phong);

  shader.SetBool("gamma", g_gamma);

  shader.SetMat4("view", camera.GetViewMatrix());
  shader.SetMat4("project", glm::perspective(glm::radians(45.0f), (float)g_width / g_height, 0.1f, 100.0f));

  shader.SetVec3("view_position", camera.GetPosition());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, material.GetDiffuse1().GetID());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, material.GetSpecular1().GetID());

  shader.SetInt("material.diffuse1", 0);
  shader.SetInt("material.specular1", 1);
  shader.SetFloat("material.shininess", (float)material.GetShininess());

  shader.SetBool("direct_light.enable", direct_light.IsEnabled());
  shader.SetVec3("direct_light.direction", direct_light.GetDirection());
  shader.SetVec3("direct_light.ambient", direct_light.GetAmbient());
  shader.SetVec3("direct_light.diffuse", direct_light.GetDiffuse());
  shader.SetVec3("direct_light.specular", direct_light.GetSpecular());

  for (std::size_t i = 0; i < sizeof(point_light) / sizeof(PointLight); ++i) {
    shader.SetBool("point_light[" + std::to_string(i) + "].enable", point_light[i].IsEnabled());
    shader.SetVec3("point_light[" + std::to_string(i) + "].position", point_light[i].GetPosition());
    shader.SetVec3("point_light[" + std::to_string(i) + "].ambient", point_light[i].GetAmbient());
    shader.SetVec3("point_light[" + std::to_string(i) + "].diffuse", point_light[i].GetDiffuse());
    shader.SetVec3("point_light[" + std::to_string(i) + "].specular", point_light[i].GetSpecular());
    shader.SetFloat("point_light[" + std::to_string(i) + "].constant", point_light[i].GetConstant());
    shader.SetFloat("point_light[" + std::to_string(i) + "].linear", point_light[i].GetLinear());
    shader.SetFloat("point_light[" + std::to_string(i) + "].quadratic", point_light[i].GetQuadratic());
  }

  shader.SetBool("spot_light.enable", spot_light.IsEnabled());
  shader.SetVec3("spot_light.position", camera.GetPosition());
  shader.SetVec3("spot_light.direction", camera.GetFront());
  shader.SetFloat("spot_light.cut_off", glm::cos(glm::radians(spot_light.GetCutOff())));
  shader.SetFloat("spot_light.outer_cut_off", glm::cos(glm::radians(spot_light.GetOuterCutOff())));
  shader.SetVec3("spot_light.ambient", spot_light.GetAmbient());
  shader.SetVec3("spot_light.diffuse", spot_light.GetDiffuse());
  shader.SetVec3("spot_light.specular", spot_light.GetSpecular());
  shader.SetFloat("spot_light.constant", spot_light.GetConstant());
  shader.SetFloat("spot_light.linear", spot_light.GetLinear());
  shader.SetFloat("spot_light.quadratic", spot_light.GetQuadratic());

  for (int i = 0; i < 10; ++i) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, TestCube::cube_positions[i]);
    model = glm::rotate(model, glm::radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
    shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  glm::mat4 model(1.0f);
  shader.SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 36, 6);

  glBindVertexArray(0);
}
