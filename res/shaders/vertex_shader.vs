#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 light_space_trans;

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_tex_coords;
out vec4 frag_pos_light_space;

void main() {
  gl_Position = project * view * model * vec4(pos, 1.0);
  frag_pos = vec3(model * vec4(pos, 1.0));
  frag_normal = mat3(transpose(inverse(model))) * normal;
  frag_tex_coords = tex_coords;
  frag_pos_light_space = light_space_trans * vec4(pos, 1.0);
}
