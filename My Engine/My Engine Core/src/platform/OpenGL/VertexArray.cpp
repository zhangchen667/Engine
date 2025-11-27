#include"VertexArray.h"
namespace myarcane {
	namespace opengl {
		VertexArray::VertexArray() {
			glGenVertexArrays(1, &m_VertexArrayID);
		}
		VertexArray::~VertexArray() {
			
			//删除所有缓冲区对象
			for (GLuint i = 0; i < m_Buffers.size();++i) {
				delete m_Buffers[i];
			}
			glDeleteVertexArrays(1, &m_VertexArrayID);
		}
		
		void VertexArray::bind() const {
			glBindVertexArray(m_VertexArrayID);
		}
		void VertexArray::unbind() const {
			glBindVertexArray(0);
		}
		void VertexArray::addBuffer(Buffer* buffer, GLuint index) {
			bind();//绑定顶点数组对象
			buffer->bind();//绑定缓冲区对象
			//设置顶点属性指针
			glEnableVertexAttribArray(index);//启用顶点属性数组
			glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, buffer->getComponentCount() * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针
			buffer->unbind();
			unbind();
			m_Buffers.push_back(buffer);
		}
	}
}