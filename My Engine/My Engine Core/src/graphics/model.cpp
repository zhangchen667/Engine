#include"Model.h"
#include<iostream>
#include"../utils/Logger.h"
#include"../stb/stb_image.h"
namespace myarcane {
	namespace graphics {
		Model::Model(const char* path) {
			loadModel(path);
		}
		Model::Model(const std::vector<Mesh>& meshes) :m_Meshes(meshes) {

		}
		void Model::Draw(Shader& shader)const {
			for (unsigned int i = 0; i < m_Meshes.size(); i++) {
				m_Meshes[i].Draw(shader);
			}
		}
		void Model::loadModel(const std::string& path) {
			Assimp::Importer import;
			//读取文件
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {//检查错误
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				utils::Logger::getInstance().error("logged_files/log.txt", "model initialization", import.GetErrorString());
				return;
			}
			//获取目录路径
			m_Directory = path.substr(0, path.find_last_of('/'));
			//处理节点
			processNode(scene->mRootNode, scene);
		}
		void Model::processNode(aiNode* node, const aiScene* scene) {
			//处理节点所有网格
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				//获取网格
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_Meshes.push_back(processMesh(mesh, scene));
			}
			//递归处理子节点
			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}
		Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Texture> textures;
			//处理顶点
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;//定义顶点
				//位置
				glm::vec3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;
				//法线
				if (mesh->HasNormals()) {
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.Normal = vector;
				}
				//纹理坐标
				if (mesh->mTextureCoords[0]) {
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else {
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				}
				vertices.push_back(vertex);
			}
			//处理索引
			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}
			//处理材质
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				//漫反射贴图
				std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				//镜面反射贴图
				std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}
			return Mesh(vertices, indices, textures);
		}
		std::vector<Texture>Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
			std::vector<Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, i, &str);
				//检查纹理是否已加载
				bool skip = false;
				for (unsigned int j = 0; j < m_LoadedTextures.size(); ++j) {
					if (std::strcmp(str.C_Str(), m_LoadedTextures[j].path.C_Str()) == 0) {
						textures.push_back(m_LoadedTextures[j]);
						skip = true;
						break;
					}
				}

				if (!skip) {
					Texture texture;
					texture.id = TextureFromFile(str.C_Str(), m_Directory); // Assumption made: material stuff is located in the same directory as the model object
					texture.type = typeName;
					texture.path = str;
					textures.push_back(texture);
					m_LoadedTextures.push_back(texture); // Add to loaded textures, so no duplicate texture gets loaded
				}
			}
			return textures;
		}
		unsigned int Model::TextureFromFile(const char* path, const std::string& directory) {
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data) {
				GLenum format;
				switch (nrComponents) {
				case 1: format = GL_RED;  break;
				case 3: format = GL_RGB;  break;
				case 4: format = GL_RGBA; break;
				}

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else {
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}
			return textureID;
		}
	}
}
