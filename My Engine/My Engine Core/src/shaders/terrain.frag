#version 460 core

struct Material {
	sampler2D texture_diffuse1; // background texture
	sampler2D texture_diffuse2; // r texture
	sampler2D texture_diffuse3; // g texture
	sampler2D texture_diffuse4; // b texture
	sampler2D texture_diffuse5; // blend map//混合纹理，每个像素的RGB值来控制另外三张纹理的混合比例
											//使用多种纹理融合更加自然，方便
	float shininess;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform vec3 viewPos;

void main() {

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec4 blendMapColour = texture(material.texture_diffuse5, TexCoords);//采样混合纹理
	float backTextureAmount = 1.0 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);//计算背景纹理的权重
	//采样每个纹理并乘以对应的权重
	vec4 backgroundTextureColour = texture(material.texture_diffuse1, TexCoords*128) * backTextureAmount;//背景纹理128倍铺设
	vec4 rTextureColour = texture(material.texture_diffuse2, TexCoords*128) * blendMapColour.r;//R纹理
	vec4 gTextureColour = texture(material.texture_diffuse3, TexCoords*128) * blendMapColour.g;//G纹理
	vec4 bTextureColour = texture(material.texture_diffuse4, TexCoords*128) * blendMapColour.b;//B纹理
	vec4 terrainColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;//最终纹理颜色
	//结果
	color = terrainColour;
}