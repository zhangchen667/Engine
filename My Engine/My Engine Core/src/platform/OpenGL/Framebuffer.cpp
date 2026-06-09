#include"Framebuffer.h"
namespace myarcane {
	namespace opengl {
		Framebuffer::Framebuffer(int width, int height, bool multisampledBuffers, bool useDepthTexture)
			: m_Width(width), m_Height(height), m_DepthTexture(0)
		{
			glGenFramebuffers(1, &m_FBO);
			bind();

			// Colour texture
			glGenTextures(1, &m_ColourTexture);
			if (multisampledBuffers) {
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, GL_RGB, width, height, GL_TRUE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture, 0);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, m_ColourTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
			}

			// Depth / stencil
			if (useDepthTexture) {
				// Depth texture (sampleable by SSAO shader)
				glGenTextures(1, &m_DepthTexture);
				glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
					GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D, m_DepthTexture, 0);
				m_DepthStencilRBO = 0;
			}
			else {
				// Renderbuffer (not sampleable)
				glGenRenderbuffers(1, &m_DepthStencilRBO);
				glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRBO);
				if (multisampledBuffers) {
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLE_AMOUNT, GL_DEPTH24_STENCIL8, width, height);
				}
				else {
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				}
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRBO);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				m_DepthTexture = 0;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				utils::Logger::getInstance().error("logged_files/error.txt", "framebuffer initialization", "ERROR: Framebuffer is not complete!");
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
