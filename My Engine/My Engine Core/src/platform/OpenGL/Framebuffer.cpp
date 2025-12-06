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
			glGenRenderbuffers(1, &m_RBO);	// 创建渲染缓冲对象
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);// 绑定渲染缓冲对象
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height); // 分配存储空间
			glBindRenderbuffer(GL_RENDERBUFFER, 0);// 解绑渲染缓冲对象
			// 创建颜色缓冲纹理
			glGenTextures(1, &m_ColourTexture);// 创建纹理对象
			glBindTexture(GL_TEXTURE_2D, m_ColourTexture);// 绑定纹理对象
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);// 分配纹理存储空间
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 设置纹理参数
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //设置纹理环绕方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			// 将颜色缓冲纹理附加到帧缓冲对象
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
			// 将渲染缓冲对象附加到帧缓冲对象作为深度和模板缓冲附件
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);//m_RBO具体内容在渲染时生成

			// 检查帧缓冲是否完整
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {//检查帧缓冲是否完整
				utils::Logger::getInstance().error("logged_files/error.txt", "framebuffer initialization", "ERROR: Framebuffer is not complete!");
				return;
			}
			unbind();
		}
		void Framebuffer::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			//glViewport(0, 0, m_Width, m_Height);//设置视口大小为帧缓冲大小
		}
		void Framebuffer::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glViewport(0, 0, m_Width, m_Height);//恢复视口大小为窗口大小
		}
	}
}