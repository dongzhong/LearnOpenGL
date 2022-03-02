#version 330 core
out vec4 frag_color;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;

uniform sampler2D shadow_map;

void main() {
  float depth_value = texture(shadow_map, frag_tex_coords).r;
  frag_color = vec4(vec3(depth_value), 1.0);
}
