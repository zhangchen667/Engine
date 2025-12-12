#include"Framebuffer.h"
namespace myarcane {
	namespace opengl {
		Framebuffer::Framebuffer(int width, int height, bool multisampledBuffers) : m_Width(width), m_Height(height)
		{
			// 创建帧缓冲对象
			glGenFramebuffers(1, &m_FBO);
			bind();

			// 创建颜色缓冲纹理附件(颜色，深度，模板等缓冲附件)
			//深度和模板缓冲使用渲染缓冲对象
			glGenRenderbuffers(1, &m_DepthStencilRBO);//创建渲染缓冲对象，就是一个缓冲区
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRBO);//绑定渲染缓冲对象
			if (multisampledBuffers) {//如果使用多重采样缓冲
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLE_AMOUNT, GL_DEPTH24_STENCIL8, m_Width, m_Height);//分配渲染缓冲存储空间，使用多重采样
			}
			else {
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);//分配渲染缓冲存储空间
			}
				// 创建颜色缓冲纹理

				glGenTextures(1, &m_ColourTexture);// 创建纹理对象
				if (multisampledBuffers) {
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture);//绑定多重采样纹理
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, GL_RGB, width, height, GL_TRUE);//分配多重采样纹理存储空间
					//多重采样纹理不需要设置过滤和环绕方式，因为不能对其进行采样操作
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);//解绑纹理
				}
				else {
					glBindTexture(GL_TEXTURE_2D, m_ColourTexture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Both need to clamp to edge or you might see strange colours around the
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // border due to interpolation and how it works with GL_REPEAT
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				if (multisampledBuffers) {//附加多重采样颜色纹理到帧缓冲
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture, 0);
				}
				else {//附加颜色纹理到帧缓冲
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
				}
				
				// 检查帧缓冲是否完整
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {//检查帧缓冲是否完整
					utils::Logger::getInstance().error("logged_files/error.txt", "framebuffer initialization", "ERROR: Framebuffer is not complete!");
					//return;
				}
				unbind();
		}
		
		Framebuffer::~Framebuffer() {

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