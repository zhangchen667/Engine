#pragma once
#include<vector>
#include"../platform/OpenGL/VertexArray.h"
#include"../platform/OpenGL/IndexBuffer.h"
#include"../platform/OpenGL/Utility.h"
#include"../platform/OpenGL/Buffer.h"
#include"Shader.h"
#include"camera/FPSCamera.h"
#include"Window.h"
namespace myarcane {
	namespace graphics {
		//天空盒类
		class Skybox {
		public:
			Skybox(const std::vector<const char*>& faces, FPSCamera* camera, Window* window);//构造函数,传入六个面的文件路径和摄像机指针和窗口指针
			~Skybox();
			void Draw();//渲染天空盒
		private:
			opengl::VertexArray m_SkyboxVAO;
			opengl::IndexBuffer m_SkyboxIBO;
			opengl::Buffer  m_SkyboxVBO;
			FPSCamera* m_Camera;
			Window* m_Window;
			Shader m_SkyboxShader;

			unsigned int m_SkyboxCubemap;//天空盒立方体贴图ID
		
		};
	}
}