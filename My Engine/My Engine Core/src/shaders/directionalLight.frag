#version 460 core

struct Material{//物体材质
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};
struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//方向光暂时不算距离衰减
};
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;//世界坐标系下的片段位置

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;//摄像机位置
uniform float time;
void main(){
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(-light.direction);//光源方向是与光照方向相反的(物体指向光源的向量)
	//环境光
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
	//漫反射
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse =light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
	//镜面反射
	vec3 viewDir=normalize(viewPos-FragPos);//物体指向摄像机的向量
	vec3 reflectDir=reflect(-lightDir,norm);//反射向量
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);//数越大高光越集中
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;
	//自发光(与光源无关)
	vec3 emission=texture(material.emission,TexCoords).rgb*clamp((sin(time)*2)-1,0,1);//让其发光强度随时间变化
	//结果
	vec3 result=ambient+diffuse+specular+emission;
	FragColor=vec4(result,1.0f);
}