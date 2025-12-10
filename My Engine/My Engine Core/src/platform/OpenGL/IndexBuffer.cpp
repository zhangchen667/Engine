#include"IndexBuffer.h"
namespace myarcane{
	namespace opengl {
		IndexBuffer::IndexBuffer()
		{
			glGenBuffers(1, &m_BufferID);//生成一个缓冲区对象
			//m_Count = 0;
		}
		IndexBuffer::IndexBuffer(GLuint* data, GLsizei amount)
		{
			glGenBuffers(1, &m_BufferID);//生成一个缓冲区对象
			load(data, amount);//加载索引数据
		}
		IndexBuffer::~IndexBuffer() {
			glDeleteBuffers(1, &m_BufferID);
		}
		void IndexBuffer::load(GLuint* data, GLsizei amount) {
			m_Count = amount;//更新索引数量
			bind();//绑定缓冲区
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, amount * sizeof(GLuint), data, GL_STATIC_DRAW);//加载顶点数据
			unbind();//解绑缓冲区
		}
		void IndexBuffer::bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		}

		void IndexBuffer::unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}