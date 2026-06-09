#version 460 core

struct Material{//�������
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};
struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;//���й����ǵ�����ֵ
	float outerCutOff;//���й����ǵ�����ֵ

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;//��������ϵ�µ�Ƭ��λ��

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;//�����λ��
uniform float time;

void main(){
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(light.position-FragPos);//����ָ���Դ������
	//������
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	//������
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse =light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	//���淴��
	vec3 viewDir=normalize(viewPos-FragPos);//����ָ�������������
	vec3 reflectDir=reflect(-lightDir,norm);//��������
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);//��Խ��߹�Խ����
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;
	//�Է���(���Դ�޹�)
	vec3 emission=texture(material.emission,TexCoords).rgb*clamp((sin(time)*2)-1,0,1);//���䷢��ǿ����ʱ��仯
	//�۹�˥��
	float theta=dot(lightDir,normalize(-light.direction));//����Ƭ��λ����۹ⷽ��ļн�����ֵ
	float difference=light.cutOff-light.outerCutOff;//�����й���������ֵ֮��
	float intensity=clamp((theta-light.outerCutOff)/difference,0.0,1.0);//����ǿ��
	diffuse*=intensity;
	specular*=intensity;
	//����˥��
	float distance=length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	//���
	vec3 result=ambient+diffuse+specular+emission;
	FragColor=vec4(result,1.0f);
}