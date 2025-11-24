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
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;//顶点索引顺序
			std::vector<Texture> textures;
			Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
				const std::vector<Texture>& textures);
			void Draw(Shader& shader)const;
		private:
			unsigned int m_VAO, m_VBO, m_EBO;
			void setupMesh();
		};
	}
}