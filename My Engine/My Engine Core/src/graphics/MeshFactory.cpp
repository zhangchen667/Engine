#include"MeshFactory.h"
namespace myarcane {
	namespace graphics {
		Mesh* MeshFactory::CreateQuad(const char* path, bool shouldHaveSpec) {//创建一个四边形网格
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Texture> textures;
			//定义四个顶点

			vertices.push_back(Vertex(glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
			vertices.push_back(Vertex(glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1)));
			vertices.push_back(Vertex(glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
			vertices.push_back(Vertex(glm::vec3(1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(1, 0)));

			//定义两个三角形的顶点索引
			indices.push_back(0);indices.push_back(1);indices.push_back(2);
			indices.push_back(2);indices.push_back(1);indices.push_back(3);
			//加载纹理
			Texture texture;
			texture.id = opengl::Utility::loadTextureFromFile(path,true);
			texture.type = "texture_diffuse";
			textures.push_back(texture);
			if (shouldHaveSpec) {
				texture.id = opengl::Utility::loadTextureFromFile("res/textures/fullSpec.png");
			}
			else {
				texture.id = opengl::Utility::loadTextureFromFile("res/textures/noSpec.png");
			}
			texture.type = "texture_specular";
			textures.push_back(texture);
			return new Mesh(vertices, indices, textures);
		}
		Mesh* MeshFactory::CreateScreenQuad(int colourBufferId) {
			std::vector<Vertex>vertices;
			std::vector<unsigned int>indices;
			std::vector<Texture>textures;
			//定义四个顶点
			vertices.push_back(Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)));
			vertices.push_back(Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1)));
			vertices.push_back(Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
			vertices.push_back(Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 0)));
			//定义indices
			indices.push_back(1); indices.push_back(0); indices.push_back(2);
			indices.push_back(3); indices.push_back(1); indices.push_back(2);
			//加载纹理
			Texture texture;
			texture.id = colourBufferId;
			texture.type = "texture_diffuse";
			textures.push_back(texture);
			return new Mesh(vertices, indices, textures);
		}
	}
}