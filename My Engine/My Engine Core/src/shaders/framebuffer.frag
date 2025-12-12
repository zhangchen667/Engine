#version 460 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture_diffuse1;

const float readOffset=1.0/400.0;//偏移量，根据屏幕分辨率调整
void main(){
	//采样当前像素和周围像素的颜色值
	vec2 readOffsets[9]=vec2[](//9个偏移量
	vec2(-readOffset, readOffset), 
	vec2(0.0, readOffset),
	vec2(readOffset, readOffset),
	vec2(-readOffset, 0.0),
	vec2(0.0, 0.0),
	vec2(readOffset, 0.0),
	vec2(-readOffset, -readOffset),
	vec2(0.0, -readOffset),
	vec2(readOffset, -readOffset)
	);
	//定义卷积核
	float kernel[9]=float[](
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	);
	vec3 colour=vec3(0.0);//初始化颜色值
	for(int i=0;i<9;i++){
		vec3 sampleTex=texture(texture_diffuse1, TexCoord + readOffsets[i]).rgb;
		colour+=sampleTex * kernel[i];
	}
	FragColor=vec4(colour, 1.0);
}