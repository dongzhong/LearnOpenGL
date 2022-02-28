#version 330 core
out vec4 frag_color;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;

struct Material {
  sampler2D diffuse1;
  sampler2D specular1;
  float shininess;
};

struct DirectLight {
  bool enable;

  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  bool enable;

  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  bool enable;

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

uniform bool is_blinn_phong;

uniform bool gamma = true;

uniform vec3 view_position;

uniform Material material;

uniform DirectLight direct_light;
uniform PointLight point_light[POINT_LIGHT_COUNT];
uniform SpotLight spot_light;

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view_dir);

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_position, vec3 view_dir);

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_position, vec3 view_dir);

void main() {
  vec3 normal = normalize(frag_normal);
  vec3 view_direction = normalize(view_position - frag_pos);

  vec3 res = vec3(0.0, 0.0, 0.0);

  if (direct_light.enable) {
    res = CalculateDirectLight(direct_light, normal, view_direction);
  }

  for (int i = 0; i < POINT_LIGHT_COUNT; ++i) {
    if (point_light[i].enable) {
      res += CalculatePointLight(point_light[i], normal, frag_pos, view_direction);
    }
  }

  if (spot_light.enable) {
    res += CalculateSpotLight(spot_light, normal, frag_pos, view_direction);
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
  if (is_blinn_phong) {
    vec3 view_direction = normalize(view_position - frag_pos);
    vec3 half_vec = normalize(light_dir + view_direction);
    spec = pow(max(dot(normal, half_vec), 0.0), material.shininess);
  } else {
    vec3 reflect_dir = reflect(-light_dir, normal);
    spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  }
  vec3 specular = light.specular * spec * specular1;

  return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_position, vec3 view_dir) {
  vec3 diffuse1 = vec3(texture(material.diffuse1, frag_tex_coords));
  vec3 specular1 = vec3(texture(material.specular1, frag_tex_coords));

  vec3 ambient = light.ambient * diffuse1;

  vec3 light_dir = normalize(light.position - frag_position);
  float diff = max(dot(light_dir, normal), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuse1;

  float spec;
  if (is_blinn_phong) {
    vec3 view_direction = normalize(view_position - frag_position);
    vec3 half_vec = normalize(light_dir + view_direction);
    spec = pow(max(dot(normal, half_vec), 0.0), material.shininess);
  } else {
    vec3 reflect_dir = reflect(-light_dir, normal);
    spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  }
  vec3 specular = light.specular * spec * specular1;

  float distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_position, vec3 view_dir) {
  vec3 diffuse1 = vec3(texture(material.diffuse1, frag_tex_coords));
  vec3 specular1 = vec3(texture(material.specular1, frag_tex_coords));

  vec3 ambient = light.ambient * diffuse1;

  vec3 light_dir = normalize(light.position - frag_pos);
  float theta = dot(light_dir, normalize(-light.direction));
  float epsilon = light.cut_off - light.outer_cut_off;
  float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

  float diff = max(dot(light_dir, normal), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuse1 * intensity;

  float spec;
  if (is_blinn_phong) {
    vec3 view_direction = normalize(view_position - frag_position);
    vec3 half_vec = normalize(light_dir + view_direction);
    spec = pow(max(dot(normal, half_vec), 0.0), material.shininess);
  } else {
    vec3 reflect_dir = reflect(-light_dir, normal);
    spec = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
  }
  vec3 specular = light.specular * spec * specular1 * intensity;

  float distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  return (ambient + diffuse + specular) * attenuation;
}
