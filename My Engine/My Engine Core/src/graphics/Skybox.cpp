#include"Skybox.h"
namespace myarcane {
	namespace graphics {
		Skybox::Skybox(const std::vector<const char*>& faces, FPSCamera* camera, Window* window)
			:m_Camera(camera), m_Window(window), m_SkyboxShader("src/shaders/skybox.vert", "src/shaders/skybox.frag")
		{
			m_SkyboxCubemap = opengl::Utility::loadCubemapFromFiles(faces);//加载立方体贴图
			//天空盒顶点数据
			GLfloat skyboxVertices[] = {
				// Front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// Back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0
			};
			GLuint skyboxIndices[] = {
				// front
			2, 1, 0,
			0, 3, 2,
			// top
			6, 5, 1,
			1, 2, 6,
			// back
			5, 6, 7,
			7, 4, 5,
			// bottom
			3, 0, 4,
			4, 7, 3,
			// left
			1, 5, 4,
			4, 0, 1,
			// right
			6, 2, 3,
			3, 7, 6,
			};
			//设置天空盒VAO,VBO,IBO,IBO是索引缓冲区对象
			m_SkyboxVAO.bind(); // 关键：提前绑定VAO，让IBO绑定关联到VAO
			m_SkyboxVBO.load(skyboxVertices, 8 * 3, 3);//加载顶点数据,每个顶点3个分量,共8个顶点
			m_SkyboxIBO.load(skyboxIndices,36);//加载索引数据
			m_SkyboxVAO.addBuffer(&m_SkyboxVBO,0);//设置顶点属性指针
			m_SkyboxVAO.unbind(); // 解绑VAO，保存状态
		}
		void Skybox::Draw(){
			m_SkyboxShader.enable();
			glActiveTexture(GL_TEXTURE0);
			m_SkyboxShader.setUniform1i("skyboxCubemap", 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxCubemap);

			m_SkyboxShader.setUniformMat4("view", m_Camera->getViewMatrix());
			m_SkyboxShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFov()), (float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 100.0f));

			
			glDepthFunc(GL_LEQUAL);//将深度函数设置为GL_LEQUAL，以确保天空盒能够正确渲染在其他对象的后面
			m_SkyboxVAO.bind();
			m_SkyboxIBO.bind();
			glDrawElements(GL_TRIANGLES, m_SkyboxIBO.getCount(), GL_UNSIGNED_INT, 0);//绘制天空盒
			m_SkyboxVAO.unbind();
			m_SkyboxIBO.unbind();
			
			glDepthFunc(GL_LESS);//将深度函数设置为GL_LESS，以确保后续渲染的对象能够正确进行深度测试
			m_SkyboxShader.disable();
		}
	}
}