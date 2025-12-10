#pragma once
//该文件用于OpenGL相关的工具函数，如纹理加载等
#include<string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"../../stb/stb_image.h"
#include<iostream>
#include<vector>
#include"../../utils/Logger.h"
namespace myarcane {
	namespace opengl {
		class Utility {
		public:
			static GLuint loadTextureFromFile(const char* path, bool containsTransparencyOnSides=false);//从文件加载纹理,如果纹理在侧面包含透明度则设置相应参数
			static GLuint loadCubemapFromFiles(const std::vector<const char*> &vec);//从六个文件加载立方体贴图
		};
	}
}