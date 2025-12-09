#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"../../utils/Logger.h"
namespace myarcane {
	namespace opengl {
		class Framebuffer {
		public:
			Framebuffer(int width, int height);
			void bind();//绑定帧缓冲
			void unbind();//解绑帧缓冲，恢复默认帧缓冲
			inline GLuint getColourBufferTexture() const { return m_ColourTexture; }//获取颜色缓冲纹理ID
			inline GLuint getDepthStencilBufferTexture() { return m_DepthStencilTexture; }//获取深度模板缓冲纹理ID
		private:
			GLuint m_FBO;// 帧缓冲对象ID
			GLuint m_ColourTexture;// 颜色缓冲纹理ID,纹理附件，比如GL_COLOR_ATTACHMENT0
			GLuint m_DepthStencilTexture; //渲染缓冲对象ID,深度和模板缓冲附件
			unsigned m_Width, m_Height;//帧缓冲宽度和高度
		};
	}
}