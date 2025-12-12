#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 texCoords;//问题出在这里，location要和mesh中的draw函数传入的参数对应上
out vec2 TexCoord;

void main(){
	gl_Position=vec4(aPos,1.0);
	TexCoord=texCoords;
}