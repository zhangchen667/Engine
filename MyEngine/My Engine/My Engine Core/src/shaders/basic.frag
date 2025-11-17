#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPosition;
uniform vec3 viewPos;
void main(){
	//环境光
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColour;

	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	//镜面反射
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 反射向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64); //数越大高光越集中
	vec3 specular = specularStrength * spec * lightColour;

	// 结果
	vec3 result = (ambient + diffuse + specular) * objectColour;
	FragColor = vec4(result, 1.0f);
}