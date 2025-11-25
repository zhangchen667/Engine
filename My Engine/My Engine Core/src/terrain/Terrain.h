#pragma once
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<glm/glm.hpp>
#include"../graphics/Mesh.h"
#include"../graphics/Shader.h"
namespace myarcane {
	namespace terrain {
		class Terrain {
		private:
			//地形参数
			GLfloat m_TerrainSize;//地形尺寸
			GLuint m_VertexSideCount;//每边顶点数量
			
			glm::vec3 m_Position;//地形世界位置
			graphics::Mesh* m_Mesh;//地形网格
		public:
			Terrain(glm::vec3& worldPosition);
			~Terrain();
			void Draw(graphics::Shader& shader)const;
		};
	}
}