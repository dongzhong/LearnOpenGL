#version 330 core
out vec4 frag_color;

in vec2 our_tex;
in vec3 our_normal;
in vec3 frag_pos;

uniform vec3 light;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform sampler2D our_texture1;

void main() {
  vec4 object_color = texture(our_texture1, our_tex);

  float ambient_trength = 0.2;
  vec3 ambient = ambient_trength * light;

  vec3 norm = normalize(our_normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = diff * light;

  float specular_strength = 0.5;
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specular_strength * spec * light;

  vec3 light_col = ambient + diffuse + specular;
  frag_color = object_color * vec4(light_col, 1.0);
}
