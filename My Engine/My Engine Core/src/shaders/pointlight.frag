#version 460 core
struct Material{//物体材质
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//距离衰减因子
	float constant;// 常数衰减因子
	float linear;// 线性衰减因子
	float quadratic;// 二次衰减因子
}
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;//世界坐标系下的片段位置
out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform float time;

void main(){
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(light.position-FragPos);//物体指向光源的向量
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
	vec3 emission=texture(material.emission,TexCoords).rgb+abs(sin(time));//让其发光强度随时间变化
	//距离衰减
	float distance=length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	//结果
	vec3 result=ambient+diffuse+specular+emission;
	FragColor=vec4(result,1.0f);
}