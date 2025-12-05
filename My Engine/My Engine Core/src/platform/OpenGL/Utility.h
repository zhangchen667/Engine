#pragma once
#include<string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"../../stb/stb_image.h"
#include<iostream>
namespace myarcane {
	namespace opengl {
		class Utility {
		public:
			static GLuint loadTextureFromFile(const char* path, bool containsTransparencyOnSides=false);//从文件加载纹理,如果纹理在侧面包含透明度则设置相应参数
		};
	}
}