#include"Mesh.h"
#include<sstream>
namespace myarcane {
	namespace graphics {
		Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
		{
			this->m_Vertices = vertices;
			this->m_Indices = indices;
			this->m_Textures = textures;
			setupMesh();
		}
		void Mesh::setupMesh() {
			//创建缓冲区对象
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);
			//绑定VAO
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
			//绑定EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

			//顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			//顶点法线
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			//顶点纹理坐标
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
			glBindVertexArray(0);
		}
		void Mesh::Draw(Shader& shader)const{
			//绑定纹理
			unsigned int diffuseNr = 1;//漫反射贴图计数
			unsigned int specularNr = 1;//镜面反射贴图计数
			for (unsigned int i = 0; i < m_Textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);//激活相应的纹理单元
				//取得纹理序号
				std::stringstream ss;
				std::string number;
				std::string name = m_Textures[i].type;
				if (name == "texture_diffuse")
					ss << diffuseNr++;//递增
				else if (name == "texture_specular")
					ss << specularNr++;//递增
				number = ss.str();//转换为字符串
				//设置采样器到对应的纹理单元
				shader.setUniform1i(("material." + name + number).c_str(), i);//设置采样器i代表第i个纹理单元
				//绑定纹理
				glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
			}
			//绘制网格
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		void Mesh::DrawFramebufferQuad(Shader& shader) const {
			// 仅绑定纹理，不拼接material前缀
			for (unsigned int i = 0; i < m_Textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				// 直接用纹理类型名作为uniform名，无material.前缀
				shader.setUniform1i(m_Textures[i].type.c_str(), i);
				glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
			}
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}