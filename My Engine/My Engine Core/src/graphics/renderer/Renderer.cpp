#include"Renderer.h"
namespace myarcane {
	namespace graphics
	{
		Renderer::Renderer(FPSCamera*camera):m_Camera(camera)
		{
		}
		void Renderer::submitOpaque(Renderable3D* renderable)
		{
			m_OpaqueRenderQueue.push_back(renderable);
		}
		void Renderer::submitTransparent(Renderable3D* renderable)
		{
			m_TransparentRenderQueue.push_back(renderable);
		}
		void Renderer::flushOpaque(Shader& shader, Shader& outlineShader)
		{
			glEnable(GL_CULL_FACE);//启用面剔除
			while (!m_OpaqueRenderQueue.empty())
			{
				glEnable(GL_DEPTH_TEST);//启用深度测试
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//设置模板测试操作,当前渲染对象通过模板测试时，更新模板缓冲区的值为参考值

				glStencilFunc(GL_ALWAYS, 1, 0xFF);//设置模板测试函数,所有片段都通过模板测试
				glStencilMask(0xFF);//启用模板写入，允许更新模板缓冲区

				Renderable3D* current = m_OpaqueRenderQueue.front();

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
					glClear(GL_STENCIL_BUFFER_BIT);
				}

				m_OpaqueRenderQueue.pop_front();
			}
		}
		void Renderer::flushTransparent(Shader& shader, Shader& outlineShader){
			glDisable(GL_CULL_FACE);//禁用面剔除
			//透明对象渲染，按照与相机的距离从远到近排序后再渲染
			std::sort(m_TransparentRenderQueue.begin(), m_TransparentRenderQueue.end(),
				[this](Renderable3D* a, Renderable3D* b)->bool {//比较距离的平方
					return (glm::length2(m_Camera->getPosition() - a->getPosition()) > glm::length2(m_Camera->getPosition() - b->getPosition()));
				});
			
			while (!m_TransparentRenderQueue.empty()) {
			

				//启动深度测试和模板测试
				//glEnable(GL_DEPTH_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				//设置模板测试操作,当前渲染对象通过模板测试时，更新模板缓冲区的值为参考值
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				//混合设置
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置混合函数,实现透明效果

				Renderable3D* current = m_TransparentRenderQueue.front();

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, current->getPosition());
				if ((current->getRotationAxis().x != 0.0f || current->getRotationAxis().y != 0.0f || current->getRotationAxis().z != 0.0f) && current->getRadianRotation() != 0)//只有在旋转轴不为零且旋转弧度不为零时才进行旋转变换
					model = glm::rotate(model, current->getRadianRotation(), current->getRotationAxis());
				model = glm::scale(model, current->getScale());
				shader.setUniformMat4("model", model);//设置model矩阵uniform变量
				current->draw(shader);//绘制当前渲染对象
				//绘制边框
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
					glClear(GL_STENCIL_BUFFER_BIT);
				}
				glDisable(GL_BLEND);//禁用混合
				m_TransparentRenderQueue.pop_front();
			}
		}
	}
}