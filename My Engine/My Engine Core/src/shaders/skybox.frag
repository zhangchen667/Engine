#version 460 core
out vec4 FragColor;
in vec3 SampleDirection;//传入采样方向
uniform samplerCube skyboxCubemap;
void main(){
	FragColor=texture(skyboxCubemap, SampleDirection);//采样立方体贴图
}