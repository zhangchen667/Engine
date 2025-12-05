#pragma once
#include"../model.h"
#include"Renderable3D.h"
#include<deque>
//核心目的：批量渲染，提升性能
//图形渲染中，“切换渲染状态”（比如切换着色器、切换纹理）是非常耗时的操作
namespace myarcane {
	namespace graphics
	{
		class Renderer {
		public:
			Renderer();
			void submit(Renderable3D* renderable);//提交渲染对象到渲染队列,不立即渲染
			void flush(Shader& shader,Shader&outlineShader);//执行渲染队列中的所有渲染对象,并清空渲染队列
		private:
			
			std::deque<Renderable3D*> m_RenderQueue;//存储渲染队列，渲染队列是一个双端队列
		};
	}
}
