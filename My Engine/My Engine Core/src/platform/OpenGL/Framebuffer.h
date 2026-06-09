#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"../../Defs.h"
#include"../../utils/Logger.h"
namespace myarcane {
	namespace opengl {
		class Framebuffer {
		public:
			Framebuffer(int width, int height, bool multisampledBuffers = true, bool useDepthTexture = false);
			~Framebuffer();
			void bind();
			void unbind();
			inline GLuint getFramebuffer() { return m_FBO; }
			inline GLuint getColourBufferTexture() const { return m_ColourTexture; }
			inline GLuint getDepthStencilBufferTexture() const { return m_DepthStencilRBO; }
			inline GLuint getDepthTexture() const { return m_DepthTexture; }
		private:
			GLuint m_FBO;
			GLuint m_ColourTexture;
			GLuint m_DepthStencilRBO;
			GLuint m_DepthTexture;
			unsigned m_Width, m_Height;
		};
	}
}
