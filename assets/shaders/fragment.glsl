#version 330 core
out vec4 FragColor;

//uniform sampler2D smapler;
uniform vec3 model_color; 
uniform vec3 light_color;
uniform vec3 light_pos;

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

    //�����Դ��Ƭ��λ��֮��ķ�������,��ķ��������ǹ�Դλ��������Ƭ��λ������֮���������,
    //����ͬ��ϣ��ȷ����������������ת��Ϊ��λ�������������ǰѷ��ߺ����յķ������������б�׼��
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    //���Ƕ�norm��lightDir�������е�ˣ������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ�졣
    //���ֵ�ٳ��Թ����ɫ���õ��������������������֮��ĽǶ�Խ������������ͻ�ԽС
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    vec3 result = (ambient + diffuse) * model_color;
    FragColor = vec4(result, 1.0);
}