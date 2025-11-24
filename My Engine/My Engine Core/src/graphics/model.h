#pragma

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Shader.h"
#include"Mesh.h"
namespace myarcane {
	namespace graphics {
		class Model {
		public:
			Model(char* path);
			void Draw(Shader& shader)const;
		private:
			std::vector<Texture> m_LoadedTextures;//已加载的纹理，避免重复加载
			std::vector<Mesh> m_Meshes;//模型包含的网格
			std::string m_Directory;//模型文件所在目录

			void loadModel(const std::string& path);//加载模型
			void processNode(aiNode* node, const aiScene* scene);//处理节点
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);//处理网格
			//加载纹理
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
			//从文件创建纹理
			unsigned int TextureFromFile(const char* path, const std::string& directory);
		};
	}
}