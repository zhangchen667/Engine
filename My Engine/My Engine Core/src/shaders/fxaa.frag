#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sceneColor;
uniform sampler2D ssaoMap;
uniform vec2 sampleOffset;

#define FXAA_SPAN_MAX 4.0
#define FXAA_REDUCE_MUL 1.0/8.0
#define FXAA_REDUCE_MIN 1.0/128.0

float luma(vec3 c){
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec3 sampleAO(vec2 uv){
	return texture(sceneColor, uv).rgb * texture(ssaoMap, uv).r;
}

void main(){
	vec3 rgbM = sampleAO(TexCoords);
	vec3 rgbN = sampleAO(TexCoords + vec2(0.0, -sampleOffset.y));
	vec3 rgbS = sampleAO(TexCoords + vec2(0.0, sampleOffset.y));
	vec3 rgbE = sampleAO(TexCoords + vec2(sampleOffset.x, 0.0));
	vec3 rgbW = sampleAO(TexCoords + vec2(-sampleOffset.x, 0.0));

	float lumaM = luma(rgbM);
	float lumaN = luma(rgbN);
	float lumaS = luma(rgbS);
	float lumaE = luma(rgbE);
	float lumaW = luma(rgbW);

	float lumaMin = min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW)));
	float lumaMax = max(lumaM, max(max(lumaN, lumaS), max(lumaE, lumaW)));
	float lumaRange = lumaMax - lumaMin;

	if(lumaRange < max(FXAA_REDUCE_MIN, lumaMax * FXAA_REDUCE_MUL)){
		FragColor = vec4(rgbM, 1.0);
		return;
	}

	vec2 dir;
	dir.x = -((lumaE - lumaW) + (lumaE - lumaW));
	dir.y = -((lumaN - lumaS) + (lumaN - lumaS));
	dir = normalize(dir);

	vec3 rgbA = 0.5 * (
		sampleAO(TexCoords + dir * FXAA_SPAN_MAX * sampleOffset) +
		sampleAO(TexCoords - dir * FXAA_SPAN_MAX * sampleOffset)
	);
	vec3 rgbB = rgbA * 0.5 + 0.25 * (
		sampleAO(TexCoords + dir * FXAA_SPAN_MAX * 0.5 * sampleOffset) +
		sampleAO(TexCoords - dir * FXAA_SPAN_MAX * 0.5 * sampleOffset)
	);

	float lumaB = luma(rgbB);
	if(lumaB < lumaMin || lumaB > lumaMax){
		FragColor = vec4(rgbA, 1.0);
	} else {
		FragColor = vec4(rgbB, 1.0);
	}
}
