#pragma once
#include"../model.h"
#include<glm/glm.hpp>
namespace myarcane {
	namespace graphics
	{
		class Renderable3D {//三维可渲染对象封装容器
		public:
			Renderable3D(glm::vec3& position, Model* model);
			~Renderable3D();
			void draw(Shader& shader)const;
		private:
			glm::vec3 m_Position;
			Model* m_Model;
		};
	}
}