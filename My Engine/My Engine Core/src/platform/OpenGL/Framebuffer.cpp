#include"Framebuffer.h"
namespace myarcane {
	namespace opengl {
		Framebuffer::Framebuffer(int width, int height) : m_Width(width), m_Height(height)
		{
			// 创建帧缓冲对象
			glGenFramebuffers(1, &m_FBO);
			bind();

			// 创建颜色缓冲纹理附件(颜色，深度，模板等缓冲附件)
			//深度和模板缓冲使用渲染缓冲对象
			glGenTextures(1, &m_DepthStencilTexture);	// 创建渲染缓冲对象
			glBindTexture(GL_TEXTURE_2D, m_DepthStencilTexture);// 绑定渲染缓冲对象
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);// 分配渲染缓冲存储空间
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 设置渲染缓冲参数
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			// 创建颜色缓冲纹理
			glGenTextures(1, &m_ColourTexture);// 创建纹理对象
			glBindTexture(GL_TEXTURE_2D, m_ColourTexture);// 绑定纹理对象
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);// 分配纹理存储空间
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 设置纹理参数
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //设置纹理环绕方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			// 将颜色缓冲纹理附加到帧缓冲对象
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
			// 将渲染缓冲对象附加到帧缓冲对象作为深度和模板缓冲附件
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilTexture, 0);//将深度模板纹理附加到帧缓冲
			// 检查帧缓冲是否完整
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {//检查帧缓冲是否完整
				utils::Logger::getInstance().error("logged_files/error.txt", "framebuffer initialization", "ERROR: Framebuffer is not complete!");
				//return;
			}
			unbind();
		}
		void Framebuffer::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			
		}
		void Framebuffer::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
		}
	}
}