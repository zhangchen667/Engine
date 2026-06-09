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
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			Vertex() : Tangent(0.0f), Bitangent(0.0f) {}
			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords)
				:Position(position), Normal(normal), TexCoords(texCoords), Tangent(0.0f), Bitangent(0.0f) {}
			Vertex(float xPos, float yPos, float zPos,
				float xNorm, float yNorm, float zNorm,
				float xTexCoords, float yTexCoords) {
				Position.x = xPos; Position.y = yPos; Position.z = zPos;
				Normal.x = xNorm;
				Normal.y = yNorm;
				Normal.z = zNorm;
				TexCoords.x = xTexCoords; TexCoords.y = yTexCoords;
				Tangent = glm::vec3(0.0f);
				Bitangent = glm::vec3(0.0f);
			}
		};
		struct Texture {
			unsigned int id;
			std::string type;
			aiString path;
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
			std::vector<unsigned int> m_Indices;
			std::vector<Texture> m_Textures;
			void setupMesh();
		};
	}
}
