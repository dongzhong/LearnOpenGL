// Created by Dong Zhong on 2022/02/18.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "camera.h"
#include "shader.h"

unsigned int g_width = 1280;
unsigned int g_height = 720;

Camera camera(glm::vec3(0.2f, 0.3f, 3.0f));

ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

bool is_drawing_coords = true;

float delta_time = 0.0f;
float current_time = 0.0f;

void RenderGUI();

void ProcessInput(GLFWwindow* window);

void DrawCoordinates(bool draw, const Shader& shader, GLuint vao);
GLuint GenerateCoordVAO();

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

  Shader shader("vertex_shader.vs", "fragment_shader.fs");

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

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glEnable(GL_DEPTH_TEST);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawCoordinates(is_drawing_coords, coords_shader, coords_vao);

    // TODO: Draw scene

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

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();
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
