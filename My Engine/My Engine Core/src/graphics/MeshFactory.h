#pragma once

#include<vector>
#include"Mesh.h"
#include"../platform/OpenGL/Utility.h"
namespace myarcane {
	namespace graphics {
		class MeshFactory {//网格工厂类，用于创建常用网格
		public:
			Mesh* CreateQuad(const char* path, bool shouldHaveSpec = false);//创建一个四边形网格
		};
	}
}