#version 460 core
layout (location = 0)in vec3 position;//世界坐标系下的顶点位置
out vec3 SampleDirection;//传递给下一个shader的变量,例子如片段着色器

uniform mat4 view;
uniform mat4 projection;
void main(){
	vec4 pos = projection * mat4(mat3(view)) * vec4(position, 1.0f);//转换到裁剪空间,mat4(mat3(view))去除平移部分,只保留旋转部分,用于天空盒,防止天空盒跟随摄像机位置移动
	SampleDirection = position; //将位置传递给下一个shader,即采样方向
	gl_Position=pos.xyww;//将顶点位置转换到裁剪空间,这里使用xyww是为了让z值恒为1.0,即天空盒总是在最远处
}