#include"Utility.h"
#include"../../utils/Logger.h"
//加载图像库纹理
namespace myarcane {
	namespace opengl {
		GLuint Utility::loadTextureFromFile(const char* path, bool containsTransparencyOnSides) {
			GLuint textureID;
			glGenTextures(1, &textureID);
			int width, height, nrComponents;
			unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
			if (data) {
				GLenum format;
				switch (nrComponents) {
				case 1:
					format = GL_RED;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
				default:
					format = GL_RGB;
					break;
				}
				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				

				if (containsTransparencyOnSides) {
					//如果纹理在侧面包含透明度则设置相应参数
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//设置环绕方式为夹边模式,防止边缘出现透明像素
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				else {//否则设置为重复模式
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//线性过滤，不能mipmap，因为mipmap只对缩小过滤有效
				//生成mipmap
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);//设置LOD偏移量为0，保持默认mipmap级别,
				//lod是mipmap级别的缩放因子，负值会使纹理看起来更清晰，正值会使纹理看起来更模糊
				//如果需要更清晰的纹理可以设置为负值，如-0.5f，但过大的负值可能会导致性能问题
				GLfloat maxAnisotropy;//各向异性过滤等级,各向异性就是根据视角不同调整纹理采样的方式,提高斜视角下的纹理质量
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);//获取最大各向异性过滤等级,为了不超过硬件支持的最大值
				GLfloat anistropyAmount = glm::min(maxAnisotropy, ANISOTROPIC_FILTERING);//使用Defs.h中定义的各向异性过滤等级
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropyAmount);//设置各向异性过滤等级
				

				stbi_image_free(data);
			}
			else {
				std::cout << "Texture failed to load at path: " << path << std::endl;
				utils::Logger::getInstance().error("logged_files/texture_loading.txt", "texture load (OpenGL) fail path:", path);
				stbi_image_free(data);
			}
			return textureID;
		}
		GLuint Utility::loadCubemapFromFiles(const std::vector<const char*>& filePaths) {
			
			if (filePaths.size() != 6) {//确保提供了6个面
				utils::Logger::getInstance().error("logged_files / error.txt", "Cubemap initialization", "Could not initialize the cubemap since 6 faces were not provided");
				return -1;
			}

			GLuint cubemapID;
			glGenTextures(1, &cubemapID);//生成纹理ID
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);//绑定立方体贴图

			int width, height, nrComponents;
			for (unsigned int i = 0; i < 6; ++i) {
				unsigned char* data = stbi_load(filePaths[i], &width, &height, &nrComponents, 0);
				if (data) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					stbi_image_free(data);
				}
				else {
					utils::Logger::getInstance().error("logged_files / error.txt", "Cubemap initialization", "Couldn't load cubemap filepath");
					stbi_image_free(data);
					return -1;
				}
			}

			//设置纹理过滤参数
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//设置纹理环绕参数
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return cubemapID;//返回立方体贴图ID
		}
	}
}