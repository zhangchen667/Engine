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
			static GLuint loadTextureFromFile(const char* path);
		};
	}
}