#version 460 core
in vec3 FragPos;
in vec3 Normal;
out vec4 colour;
uniform vec3 cameraPos;
uniform samplerCube environmentMap;
void main(){
	float refractionIndex= 1.00f / 1.52f;//空气折射率/玻璃折射率
	vec3 fragToCam = normalize(FragPos - cameraPos);
	vec3 refracteDir = refract(fragToCam, normalize(Normal), refractionIndex);//折射方向,Snell定律,入射方向,法线,折射率

	colour = texture(environmentMap, refracteDir);
}