#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 
uniform Material material;

in vec2 TexCoords;

struct Light {
    vec3 position;  
    vec3 direction;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform vec3 view_pos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 环境光
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //光的衰减
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}