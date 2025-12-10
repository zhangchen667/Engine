#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include"../model.h"
#include"Renderable3D.h"
#include<deque>
#include"../camera/FPSCamera.h"
#include<glm/gtx/norm.hpp>
//核心目的：批量渲染，提升性能
//图形渲染中，“切换渲染状态”（比如切换着色器、切换纹理）是非常耗时的操作
namespace myarcane {
	namespace graphics
	{
		class Renderer {
		public:
			Renderer(FPSCamera*camera);
			void submitOpaque(Renderable3D* renderable);//提交不透明渲染对象到渲染队列,不立即渲染
			void submitTransparent(Renderable3D* renderable);//提交透明渲染对象到渲染队列,不立即渲染
			void flushOpaque(Shader& shader, Shader& outlineShader);//渲染不透明对象队列
			void flushTransparent(Shader& shader, Shader& outlineShader);//渲染透明对象队列
		private:
			
			std::deque<Renderable3D*>  m_OpaqueRenderQueue;//不透明渲染队列
			std::deque<Renderable3D*>  m_TransparentRenderQueue;//透明渲染队列
			FPSCamera* m_Camera;//摄像机指针
		};
	}
}
