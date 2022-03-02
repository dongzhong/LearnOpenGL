#version 330 core
out vec4 frag_color;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;
in vec4 frag_pos_light_space;

struct Material {
  sampler2D diffuse1;
  sampler2D specular1;
  float shininess;

  bool is_blinn_phong;
};

struct DirectLight {
  bool enable;

  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define DIRECT_LIGHT_COUNT 1

uniform bool gamma = true;

uniform vec3 view_position;

uniform DirectLight direct_light[DIRECT_LIGHT_COUNT];

uniform Material material;

uniform sampler2D shadow_map;

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view_dir);

float CalculateShadow(vec4 light_space_frag_pos, vec3 normal, vec3 light_dir);

void main() {
  vec3 normal = normalize(frag_normal);
  vec3 view_direction = normalize(view_position - frag_pos);

  vec3 res = vec3(0.0, 0.0, 0.0);
  if (direct_light[0].enable) {
    res += CalculateDirectLight(direct_light[0], normal, view_direction);
  }

  if (gamma) {
    res = pow(res, vec3(1.0 / 2.2));
  }

  frag_color = vec4(res, 1.0);
}

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view_dir) {
  vec3 diffuse1 = vec3(texture(material.diffuse1, frag_tex_coords));
  vec3 specular1 = vec3(texture(material.specular1, frag_tex_coords));

  vec3 ambient = light.ambient * diffuse1;

  vec3 light_dir = normalize(-light.direction);

  float diff = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuse1;

  float spec;
  if (material.is_blinn_phong) {
    vec3 view_direction = normalize(view_position - frag_pos);
    vec3 half_vec = normalize(light_dir + view_direction);
    spec = pow(max(dot(normal, half_vec), 0.0), material.shininess);
  } else {
    vec3 reflect_dir = reflect(-light_dir, normal);
    spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  }
  vec3 specular = light.specular * spec * specular1;

  float shadow = CalculateShadow(frag_pos_light_space, normal, light_dir);

  return ambient + (1.0 - shadow) * (diffuse + specular);
}

float CalculateShadow(vec4 light_space_frag_pos, vec3 normal, vec3 light_dir) {
  vec3 project_pos = light_space_frag_pos.xyz / light_space_frag_pos.w;
  project_pos = project_pos * 0.5 + 0.5;

  if (project_pos.z > 1.0) {
    return 0.0;
  }

  float current_depth = project_pos.z;
  float shadow = 0.0;

  if (project_pos.z > 1.0) {
    shadow = 0.0;
  } else {
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        float closest_depth = texture(shadow_map, project_pos.xy + vec2(i, j) * texel_size).r;
        shadow += current_depth - bias > closest_depth ? 1.0 : 0.0;
      }
    }
    shadow /= 9.0;
  }

  return shadow;
}
