#version 330 core
out vec4 FragColor;

//uniform sampler2D smapler;
uniform vec3 model_color; 
uniform vec3 light_color;
uniform vec3 light_pos;

uniform vec3 view_pos;

//in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    //FragColor = texture(smapler, uv);
    //FragColor = vec4(0.1f,0.2f,0.0f,1.0f);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;
//    vec3 result = ambient * model_color;
//    FragColor = vec4(result, 1.0);
  


    //计算光源和片段位置之间的方向向量,光的方向向量是光源位置向量与片段位置向量之间的向量差,
    //我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);

    /// 漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
//    vec3 result = (ambient + diffuse) * model_color;
//    FragColor = vec4(result, 1.0);
     /// 漫反射

    /// 镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;
    vec3 result = (ambient + diffuse + specular) * model_color;
    FragColor = vec4(result, 1.0);
    /// 镜面反射


}