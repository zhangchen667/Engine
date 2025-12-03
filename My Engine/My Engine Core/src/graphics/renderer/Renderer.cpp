#include"Renderer.h"
namespace myarcane {
	namespace graphics
	{
		void Renderer::submit(Renderable3D* renderable)
		{
			m_RenderQueue.push_back(renderable);
		}
		void Renderer::flush(Shader& shader)
		{
			while (!m_RenderQueue.empty())
			{
				const Renderable3D* renderable = m_RenderQueue.front();
				renderable->draw(shader);
				m_RenderQueue.pop_front();
			}
		}
	}
}