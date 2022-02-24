#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 project;

void main() {
  gl_Position = project * view * trans * vec4(pos, 1.0);
}
