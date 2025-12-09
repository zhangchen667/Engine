#pragma once
#include <vector>
#include<glm/glm.hpp>
#include<assimp/Importer.hpp>
#include<string>
#include"Shader.h"
namespace myarcane {
	namespace graphics {
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			Vertex() {}
			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords)
				:Position(position), Normal(normal), TexCoords(texCoords) {}
		};
		struct Texture {
			unsigned int id;//纹理ID
			std::string type;//diffuse, specular
			aiString path;//纹理路径
		};
		class Mesh {
		public:
			
			Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
				const std::vector<Texture>& textures);
			void Draw(Shader& shader)const;
			void DrawFramebufferQuad(Shader& shader) const;
			inline const std::vector<Vertex> & getVertices() const { return m_Vertices; }
			inline const std::vector<unsigned int>& getIndices() const { return m_Indices; }
			inline const std::vector<Texture>& getTextures() const { return m_Textures; }
		private:
			unsigned int m_VAO, m_VBO, m_EBO;
			std::vector<Vertex> m_Vertices;
			std::vector<unsigned int> m_Indices;//顶点索引顺序
			std::vector<Texture> m_Textures;
			void setupMesh();
		};
	}
}