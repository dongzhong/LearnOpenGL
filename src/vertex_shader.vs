#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

out vec2 frag_tex;
out vec3 frag_normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main() {
  gl_Position = project * view * model * vec4(position, 1.0);
  frag_tex = tex_coords;
  frag_normal = mat3(transpose(inverse(model))) * normal;
  frag_pos = vec3(model * vec4(position, 1.0));
}
