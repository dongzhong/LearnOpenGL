#version 330 core
out vec4 frag_color;

in vec2 our_tex;
in vec3 our_normal;
in vec3 frag_pos;

uniform vec3 view_pos;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cut_off;
  float outer_cut_off;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

#define POINT_LIGHT_COUNT 4

uniform Material material;
uniform DirectLight dir_light;
uniform PointLight point_light[POINT_LIGHT_COUNT];
uniform SpotLight spot_light;

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view_dir);

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_position, vec3 view_dir);

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_position, vec3 view_dir);

void main() {
  vec3 norm = normalize(our_normal);
  vec3 view_dir = normalize(view_pos - frag_pos);

  vec3 res = CalculateDirectLight(dir_light, norm, view_dir);

  for (int i = 0; i < POINT_LIGHT_COUNT; ++i) {
    res += CalculatePointLight(point_light[i], norm, frag_pos, view_dir);
  }

  res += CalculateSpotLight(spot_light, norm, frag_pos, view_dir);

  frag_color = vec4(res, 1.0);
}

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view_dir) {
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, our_tex));

  vec3 light_dir = normalize(-light.direction);

  float diff = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, our_tex));

  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, our_tex));

  return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_position, vec3 view_dir) {
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, our_tex));

  vec3 light_dir = normalize(light.position - frag_position);
  float diff = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, our_tex));

  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, our_tex));

  float distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_position, vec3 view_dir) {
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, our_tex));

  vec3 light_dir = normalize(light.position - frag_position);
  float theta = dot(light_dir, normalize(-light.direction));
  float epsilon = light.cut_off - light.outer_cut_off;
  float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

  float diff = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, our_tex)) * intensity;

  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, our_tex)) * intensity;

  float distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  return (ambient + diffuse + specular) * attenuation;
}
