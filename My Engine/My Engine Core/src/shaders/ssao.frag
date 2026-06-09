#version 460 core
out float FragColor;
in vec2 TexCoords;

uniform sampler2D depthMap;
uniform sampler2D noiseTexture;
uniform mat4 projection;
uniform mat4 invProjection;
uniform vec3 samples[64];
uniform vec2 noiseScale;

const int KERNEL_SIZE = 64;
const float RADIUS = 0.5;
const float BIAS = 0.025;

vec3 reconstructViewPos(vec2 uv, float depth){
	vec4 clipPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
	vec4 viewPos = invProjection * clipPos;
	return viewPos.xyz / viewPos.w;
}

void main(){
	float depth = texture(depthMap, TexCoords).r;
	if(depth >= 1.0){
		FragColor = 1.0;
		return;
	}

	vec3 fragPos = reconstructViewPos(TexCoords, depth);
	vec3 normal = normalize(cross(dFdx(fragPos), dFdy(fragPos)));

	vec3 randomVec = normalize(texture(noiseTexture, TexCoords * noiseScale).xyz);
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for(int i = 0; i < KERNEL_SIZE; ++i){
		vec3 samplePos = TBN * samples[i];
		samplePos = fragPos + samplePos * RADIUS;

		vec4 offset = projection * vec4(samplePos, 1.0);
		offset.xy = (offset.xy / offset.w) * 0.5 + 0.5;

		float sampleDepth = texture(depthMap, offset.xy).r;
		vec3 samplePosActual = reconstructViewPos(offset.xy, sampleDepth);

		float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(fragPos.z - samplePosActual.z));
		occlusion += (samplePosActual.z >= samplePos.z + BIAS ? 1.0 : 0.0) * rangeCheck;
	}

	FragColor = 1.0 - (occlusion / KERNEL_SIZE);
}
