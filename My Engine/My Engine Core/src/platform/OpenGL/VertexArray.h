#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Buffer.h"
#include<vector>
namespace myarcane {
	namespace opengl {
		//VAO: Vertex Array Object 顶点数组对象
		class VertexArray {
		private:
			GLuint m_VertexArrayID;//顶点数组对象ID
			std::vector<Buffer*> m_Buffers;//存储缓冲区对象的指针
		public:
			VertexArray();
			~VertexArray();
			void bind() const;
			void unbind() const;
			//添加缓冲区对象并设置顶点属性指针
			void addBuffer(Buffer* buffer, GLuint index);
		};
	}
}