#pragma once
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"../graphics/Mesh.h"
#include"../graphics/Shader.h"
//#include"../stb/stb_image.h"
namespace myarcane {
	namespace terrain {
		class Terrain {
		private:
			//地形参数
			GLfloat m_TerrainSize;//地形尺寸
			GLuint m_VertexSideCount;//每边顶点数量
			GLfloat m_HeightMapScale;//高度图缩放

			glm::mat4 m_ModelMatrix;//地形模型矩阵,是用于定位地形的
			glm::vec3 m_Position;//地形世界位置
			graphics::Mesh* m_Mesh;//地形网格
		public:
			Terrain(glm::vec3& worldPosition);
			~Terrain();
			void Draw(graphics::Shader& shader)const;
			inline glm::vec3 getPosition() { return m_Position; };
		private:
			glm::vec3 calculateNormal(int x, int z, unsigned char* heightMapData);
			GLfloat getVertexHeight(int x, int y, unsigned char* heightMapData);
		};
	}
}