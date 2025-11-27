#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
namespace myarcane {
	namespace opengl {
		//VBO: Vertex Buffer Object 顶点缓冲区对象
		class Buffer {
		private:
			GLuint m_BufferID;//缓冲区ID
			GLuint m_ComponentCount;//每个顶点的分量数量(就是纹理（2）+位置（3）+颜色（3||4）等等的值)
		public:
			Buffer(GLfloat* data, GLsizei amount, GLuint componentCount);//amount表示顶点数量, componentCount表示每个顶点的分量数量
			~Buffer();
			void bind() const;
			void unbind() const;
			inline GLuint getComponentCount() const { return m_ComponentCount; };
		};
	}
}