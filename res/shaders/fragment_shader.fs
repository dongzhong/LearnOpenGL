#version 330 core
out vec4 frag_color;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;

void main() {
  frag_color = vec4(frag_normal, 1.0f);
}
