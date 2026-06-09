#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	TexCoords = texCoords;

	vec3 N = normalize(mat3(transpose(inverse(model))) * normal);
	vec3 T;
	if(length(tangent) > 0.001){
		T = normalize(mat3(transpose(inverse(model))) * tangent);
		T = normalize(T - dot(T, N) * N);
	} else {
		vec3 up = abs(N.y) < 0.999 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
		T = normalize(cross(up, N));
	}
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);
}
