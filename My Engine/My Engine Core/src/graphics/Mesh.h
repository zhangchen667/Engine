#pragma
#include <vector>
#include<glm/glm.hpp>
#include<assimp/Importer.hpp>
#include<string>
#include"Shader.h"
namespace myarcane {
	namespace graphics {
		struct Vertex {
			glm::vec3 Position;
			glm::vec2 Normal;
			glm::vec2 TexCoords;
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

			inline std::vector<Vertex> const& getVertices() const { return m_Vertices; }
			inline std::vector<unsigned int> const& getIndices() const { return m_Indices; }
			inline std::vector<Texture> const& getTextures() const { return m_Textures; }
		private:
			unsigned int m_VAO, m_VBO, m_EBO;
			std::vector<Vertex> m_Vertices;
			std::vector<unsigned int> m_Indices;//顶点索引顺序
			std::vector<Texture> m_Textures;
			void setupMesh();
		};
	}
}