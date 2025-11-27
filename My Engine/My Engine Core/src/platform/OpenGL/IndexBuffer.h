#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
namespace myarcane {
	namespace opengl {
		//EBO: Element Buffer Object 索引缓冲区对象
		class IndexBuffer {
		private:
			GLuint m_BufferID;//缓冲区ID
			GLsizei m_Count;//索引数量
		public:
			IndexBuffer(GLuint* data, GLsizei count);//count表示索引数量
			~IndexBuffer();
			void bind() const;
			void unbind() const;
			inline GLsizei getCount() const { return m_Count; };
		};
	}
}