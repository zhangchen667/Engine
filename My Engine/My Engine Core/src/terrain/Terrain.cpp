#include"Terrain.h"
#include"../platform/OpenGL/Utility.h"
#include"../utils/Logger.h"
namespace myarcane {
	namespace terrain {
		Terrain::Terrain(glm::vec3& worldPosition) :m_Position(worldPosition)
		{
			m_ModelMatrix = glm::translate(m_ModelMatrix, worldPosition);//设置地形位置
			std::vector<graphics::Vertex>vertices;
			std::vector<unsigned int>indices;
			std::vector<graphics::Texture>textures;
			// 高度图加载
			GLint mapWidth, mapHeight, nrComponents;
			unsigned char* heightMapImage = stbi_load("res/terrain/heightMap.png", &mapWidth, &mapHeight, &nrComponents, 1);
			if (mapWidth != mapHeight) {//高度图必须是正方形//

				//问题所在

				std::cout << "ERROR: Can't use a heightmap with a different width and height" << std::endl;
				utils::Logger::getInstance().error("logged_files/terrain_creation.txt", "terrain initialization", "Can't use a heightmap with a different width and height");
				//stbi_image_free(heightMapImage);
				return;
			}

			// Map Information
			m_VertexSideCount = mapWidth;//假设高度图是正方形的，宽度等于高度
			m_TerrainSize = 4;//每个网格的大小
			m_HeightMapScale = 100;//高度图缩放倍数
			
			for (unsigned int z = 0; z < m_VertexSideCount; z++) {
				for (unsigned int x = 0; x < m_VertexSideCount; x++) {
					graphics::Vertex vertex;
					vertex.Position = glm::vec3(x * m_TerrainSize, getVertexHeight(x, z, heightMapImage), z * m_TerrainSize);
					vertex.Normal = calculateNormal(x, z, heightMapImage);
					vertex.TexCoords = glm::vec2((GLfloat)x / ((GLfloat)m_VertexSideCount - 1.0f),
						(GLfloat)z / ((GLfloat)m_VertexSideCount - 1.0f));//转换纹理坐标0-1之间

					vertices.push_back(vertex);
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
			shader.setUniformMat4("model", m_ModelMatrix);
			m_Mesh->Draw(shader);
		}
		glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char* heightMapData) {
			GLfloat heightR = getVertexHeight(x + 1, z, heightMapData);//右边高度
			GLfloat heightL = getVertexHeight(x - 1, z, heightMapData);//左边高度
			GLfloat heightU = getVertexHeight(x, z + 1, heightMapData);//上边高度
			GLfloat heightD = getVertexHeight(x, z - 1, heightMapData);//下边高度

			glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
			normal = glm::normalize(normal);

			return normal;
		}

		GLfloat Terrain::getVertexHeight(int x, int z, unsigned char* heightMapData) {
			if (x < 0 || x >= m_VertexSideCount || z < 0 || z >= m_VertexSideCount) {
				return 0.0f;
			}

			// Normalize height to [-1, 1] then multiply it by the height map scale
			return ((heightMapData[x + (z * m_VertexSideCount)] / 127.5f) - 1) * m_HeightMapScale;//为什么除以 127.5？因为 255 / 127.5 = 2，这样能把 0~255 的像素值，先转换成 0~2 的范围
		}
	}
}