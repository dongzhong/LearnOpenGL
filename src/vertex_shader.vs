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
