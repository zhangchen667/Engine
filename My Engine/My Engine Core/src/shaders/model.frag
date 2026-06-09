#version 460 core

const float PI = 3.14159265359;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
	float metallic;
	float roughness;
	float ao;
};
struct DirLight {
	vec3 direction;
	vec3 color;
};
struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 color;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec3 color;
};
#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

out vec4 color;

uniform Material material;
uniform float time;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform bool useNormalMap;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

vec3 getNormal();
vec3 calcPBR(vec3 N, vec3 V, vec3 L, vec3 radiance, vec3 albedo, float metallic, float roughness);
vec3 CalcPBRDirLight(DirLight light, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness);
vec3 CalcPBRPointLight(PointLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness);
vec3 CalcPBRSpotLight(SpotLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness);

void main() {
	float textureAlpha = texture(material.texture_diffuse1, TexCoords).w;
	if(textureAlpha < 0.1) discard;

	vec3 albedo = texture(material.texture_diffuse1, TexCoords).rgb;
	vec3 N = getNormal();
	vec3 V = normalize(viewPos - FragPos);

	vec3 Lo = vec3(0.0);
	Lo += CalcPBRDirLight(dirLight, N, V, albedo, material.metallic, material.roughness);
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		Lo += CalcPBRPointLight(pointLights[i], N, V, FragPos, albedo, material.metallic, material.roughness);
	Lo += CalcPBRSpotLight(spotLight, N, V, FragPos, albedo, material.metallic, material.roughness);

	vec3 ambient = vec3(0.03) * albedo * material.ao;
	vec3 result = ambient + Lo;

	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0 / 2.2));

	color = vec4(result, textureAlpha);
}

vec3 getNormal(){
	if(useNormalMap){
		vec3 tangentNormal = texture(material.texture_normal1, TexCoords).xyz * 2.0 - 1.0;
		return normalize(TBN * tangentNormal);
	}
	return normalize(TBN[2]);
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
	return a2 / (PI * denom * denom + 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness)
		 * GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calcPBR(vec3 N, vec3 V, vec3 L, vec3 radiance, vec3 albedo, float metallic, float roughness){
	vec3 H = normalize(V + L);
	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	float D = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 kS = F;
	vec3 kD = (1.0 - metallic) * (vec3(1.0) - kS);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPBRDirLight(DirLight light, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness){
	vec3 L = normalize(-light.direction);
	return calcPBR(N, V, L, light.color, albedo, metallic, roughness);
}

vec3 CalcPBRPointLight(PointLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness){
	vec3 L = normalize(light.position - fragPos);
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	return calcPBR(N, V, L, light.color * attenuation, albedo, metallic, roughness);
}

vec3 CalcPBRSpotLight(SpotLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness){
	vec3 L = normalize(light.position - fragPos);
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	float theta = dot(L, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	return calcPBR(N, V, L, light.color * attenuation * intensity, albedo, metallic, roughness);
}
