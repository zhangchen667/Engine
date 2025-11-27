#version 460 core

struct Material {
	float shininess;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D backgroundTexture;

uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;//混合纹理，每个像素的RGB值来控制另外三张纹理的混合比例
//使用多种纹理融合更加自然，方便
void main() {

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//vec4 blendMapColour = texture(blendMap, TexCoords);
	//
	//float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	////vec2 tiledCoords = 
	//vec4 backgroundTextureColour = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	//vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
	//vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
	//vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;
	//
	//// Result
	//vec4 terrainColour = backroundTextureColour + rTextureColour + gTextureColour + bTextureColour;
	//
	//
	//// Result
	//color = terrainColour;

	color = texture(texture_diffuse1,TexCoords).rgba;
}