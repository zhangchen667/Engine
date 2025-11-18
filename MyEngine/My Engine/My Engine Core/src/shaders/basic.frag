#version 460 core

struct Material{//物体材质
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
void main(){
	//环境光
	vec3 ambient=material.ambient*light.ambient;

	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse*(diff* material.diffuse);

	//镜面反射
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 反射向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //数越大高光越集中
	vec3 specular = light.specular * spec * material.specular;

	// 结果
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}