#pragma
#include <vector>
#include<glm/glm.hpp>
#include<string>
#include"Shader.h"
namespace myarcane {
	namespace graphics {
		struct vertex {
			glm::vec3 Position;
			glm::vec2 Normal;
			glm::vec2 TexCoords;
		};
		struct Texture {
			unsigned int id;//ÎÆÀíID
			std::string type;//diffuse, specular
		};
		class Mesh {
		public:
			std::vector<vertex> vertices;
			std::vector<unsigned int> indices;//¶¥µãË÷ÒýË³Ðò
			std::vector<Texture> textures;
			Mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices,
				const std::vector<Texture>& textures);
			void Draw(Shader& shader)const;
		private:
			unsigned int m_VAO, m_VBO, m_EBO;
			void setupMesh();
		};
	}
}