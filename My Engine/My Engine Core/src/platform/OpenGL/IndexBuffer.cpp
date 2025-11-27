#include"IndexBuffer.h"
namespace myarcane{
	namespace opengl {
		IndexBuffer::IndexBuffer(GLuint* data, GLsizei count) :m_Count(count)
		{
			glGenBuffers(1, &m_BufferID);//生成一个缓冲区对象
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);//EBO是索引缓冲区对象
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		IndexBuffer::~IndexBuffer() {
			glDeleteBuffers(1, &m_BufferID);
		}

		void IndexBuffer::bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		}

		void IndexBuffer::unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}