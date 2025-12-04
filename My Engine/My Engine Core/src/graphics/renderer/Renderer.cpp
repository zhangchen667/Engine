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
				Renderable3D*current = m_RenderQueue.front();
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, current->getPosition());
				if ((current->getRotationAxis().x != 0.0f || current->getRotationAxis().y != 0.0f || current->getRotationAxis().z != 0.0f) && current->getRadianRotation() != 0)//只有在旋转轴不为零且旋转弧度不为零时才进行旋转变换
					model = glm::rotate(model, current->getRadianRotation(), current->getRotationAxis());
				model = glm::scale(model, current->getScale());
				shader.setUniformMat4("model", model);//设置model矩阵uniform变量
				m_RenderQueue.front()->draw(shader);//绘制当前渲染对象
				m_RenderQueue.pop_front();
			}
		}
	}
}