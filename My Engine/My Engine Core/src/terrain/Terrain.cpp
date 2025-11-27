#include"Terrain.h"
#include"../platform/OpenGL/Utility.h"
namespace myarcane {
	namespace terrain {
		Terrain::Terrain(glm::vec3& worldPosition) {
			m_Position = worldPosition;
			m_VertexSideCount = 1024;
			m_TerrainSize = 2;
			m_HeightMapScale = 100;

			std::vector<graphics::Vertex>vertices;
			std::vector<unsigned int>indices;
			std::vector<graphics::Texture>textures;
			for (unsigned int z = 0; z < m_VertexSideCount; z++) {
				for (unsigned int x = 0; x < m_VertexSideCount; x++) {
					graphics::Vertex vertex;
					vertex.Position = glm::vec3(x * m_TerrainSize, 0, z * m_TerrainSize);
					vertex.Normal = glm::vec3(0, 1, 0);
					vertex.TexCoords = glm::vec2((GLfloat)x / ((GLfloat)m_VertexSideCount - 1.0f),
						(GLfloat)z / ((GLfloat)m_VertexSideCount - 1.0f));//转换纹理坐标0-1之间

					vertices.push_back(vertex);
				}
			}
			// 高度图加载
			GLint mapWidth, mapHeight, nrComponents;
			unsigned char* heightMapImage = stbi_load("res/terrain/heightMap.png", &mapWidth, &mapHeight, &nrComponents, 1);
			

			if (!heightMapImage) {
				std::cout << "Failed to load height map image!" << std::endl;
			}
			for (GLuint height = 0; height < mapHeight; ++height) {
				for (GLuint width = 0; width < mapWidth; ++width) {
					//根据高度图调整顶点高度到0-1
					GLfloat temp = (heightMapImage[width + (height * mapWidth)] / 127.5f) - 1;
					//设置顶点高度
					vertices[width + (height * mapWidth)].Position.y = temp * m_HeightMapScale;
				}
			}
			stbi_image_free(heightMapImage);

			for (GLuint height = 0; height < m_VertexSideCount - 1; ++height) {
				for (GLuint width = 0; width < m_VertexSideCount - 1; ++width) {
					//对每个网格遍历，每个网格生成两个三角形
					// Triangle 1
					indices.push_back(width + (height * m_VertexSideCount));
					indices.push_back(1 + width + (height * m_VertexSideCount));
					indices.push_back(1 + m_VertexSideCount + width + (height * m_VertexSideCount));

					// Triangle 2
					indices.push_back(width + (height * m_VertexSideCount));
					indices.push_back(m_VertexSideCount + width + (height * m_VertexSideCount));
					indices.push_back(1 + m_VertexSideCount + width + (height * m_VertexSideCount));
				}
			}
			//加载纹理多种类混合
			graphics::Texture texture;
			texture.id = opengl::Utility::loadTextureFromFile("res/terrain/grass.png");
			texture.type = "texture_diffuse";
			textures.push_back(texture);

			texture.id = opengl::Utility::loadTextureFromFile("res/terrain/dirt.png");
			texture.type = "texture_diffuse";
			textures.push_back(texture);

			texture.id = opengl::Utility::loadTextureFromFile("res/terrain/sand.png");
			texture.type = "texture_diffuse";
			textures.push_back(texture);

			texture.id = opengl::Utility::loadTextureFromFile("res/terrain/stone.png");
			texture.type = "texture_diffuse";
			textures.push_back(texture);

			texture.id = opengl::Utility::loadTextureFromFile("res/terrain/blendMap.png");
			texture.type = "texture_diffuse";
			textures.push_back(texture);
			
			m_Mesh = new graphics::Mesh(vertices, indices, textures);//创建网格
		}
		Terrain::~Terrain() {
			delete m_Mesh;
		}
		void Terrain::Draw(graphics::Shader& shader)const {
			m_Mesh->Draw(shader);
		}
	}
}