#include"Renderer.h"
namespace myarcane {
	namespace graphics
	{
		Renderer::Renderer()
		{
		}
		void Renderer::submit(Renderable3D* renderable)
		{
			m_RenderQueue.push_back(renderable);
		}
		void Renderer::flush(Shader& shader,Shader &outlineShader)
		{
			while (!m_RenderQueue.empty())
			{
				Renderable3D*current = m_RenderQueue.front();

				glEnable(GL_DEPTH_TEST);//启用深度测试
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//设置模板测试操作,当前渲染对象通过模板测试时，更新模板缓冲区的值为参考值

				glStencilFunc(GL_ALWAYS, 1, 0xFF);//设置模板测试函数,所有片段都通过模板测试
				glStencilMask(0xFF);//启用模板写入，允许更新模板缓冲区

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, current->getPosition());
				if ((current->getRotationAxis().x != 0.0f || current->getRotationAxis().y != 0.0f || current->getRotationAxis().z != 0.0f) && current->getRadianRotation() != 0)//只有在旋转轴不为零且旋转弧度不为零时才进行旋转变换
					model = glm::rotate(model, current->getRadianRotation(), current->getRotationAxis());
				model = glm::scale(model, current->getScale());
				shader.setUniformMat4("model", model);//设置model矩阵uniform变量
				current->draw(shader);//绘制当前渲染对象
				//如果当前渲染对象需要描边效果，则进行描边渲染
				if (current->getShouldOutline()) {
					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					//设置模板测试函数,只有模板缓冲区的值不等于参考值的片段才能通过模板测试

					outlineShader.enable();
					model = glm::mat4(1.0f);
					model = glm::translate(model, current->getPosition());
					if ((current->getRotationAxis().x != 0 || current->getRotationAxis().y != 0 || current->getRotationAxis().z != 0) && current->getRadianRotation() != 0)
						//只有在旋转轴不为零且旋转弧度不为零时才进行旋转变换
						model = glm::rotate(model, current->getRadianRotation(), current->getRotationAxis());
					model = glm::scale(model, current->getScale() + glm::vec3(0.025f, 0.025f, 0.025f));//稍微放大一些以实现描边效果
					outlineShader.setUniformMat4("model", model);
					current->draw(outlineShader);
					outlineShader.disable();

					glEnable(GL_DEPTH_TEST);
					glStencilMask(0xFF);//启用模板写入，允许更新模板缓冲区

					shader.enable();
				}
				glClear(GL_STENCIL_BUFFER_BIT);
				m_RenderQueue.pop_front();
			}
		}
	}
}