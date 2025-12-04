#pragma once
#include"graphics/renderer/Renderable3D.h"
#include"graphics/camera/FPSCamera.h"
#include"graphics/renderer/Renderer.h"
#include"terrain/Terrain.h"
#include"graphics/Window.h"
namespace myarcane {
	class Scene3D {
	private:
		graphics::Window* m_Window;
		graphics::Renderer* m_Renderer;
		graphics::FPSCamera* m_Camera;
		terrain::Terrain* m_Terrain;

		std::vector<graphics::Renderable3D*> m_Renderables;//场景中的所有三维可渲染对象
		graphics::Shader terrainShader,modelShader;
	public:
		Scene3D(graphics::FPSCamera* camera, graphics::Window* window);//构造函数，传入摄像机指针和窗口指针
		~Scene3D();
		void Add(graphics::Renderable3D* renderable);//向场景中添加三维可渲染对象
		
		void onUpdate(float deltaTime);
		void onRender();
		inline graphics::FPSCamera* getCamera() { return m_Camera; }
		inline graphics::Renderer* getRenderer() { return m_Renderer; }
	private:
		void init();
	};
}