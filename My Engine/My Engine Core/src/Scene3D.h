#pragma once
#include"graphics/renderer/Renderable3D.h"
#include"graphics/camera/FPSCamera.h"
#include"graphics/renderer/Renderer.h"
#include"terrain/Terrain.h"
#include"graphics/Window.h"
#include"graphics/MeshFactory.h"
#include"graphics/Skybox.h"
namespace myarcane {
	class Scene3D {
	private:
		graphics::Window* m_Window;
		graphics::Renderer* m_Renderer;
		graphics::FPSCamera* m_Camera;
		terrain::Terrain* m_Terrain;
		graphics::MeshFactory m_MeshFactory;//网格工厂指针
		graphics::Skybox* m_Skybox;//天空盒指针,属于场景的一部分

		std::vector<graphics::Renderable3D*> m_Renderables;//场景中的所有三维可渲染对象
		graphics::Shader m_TerrainShader,m_ModelShader,m_OutlineShader,m_ModelReflectionShader;
	public:
		Scene3D(graphics::FPSCamera* camera, graphics::Window* window);//构造函数，传入摄像机指针和窗口指针
		~Scene3D();
		void Add(graphics::Renderable3D* renderable);//向场景中添加三维可渲染对象
		
		void onUpdate(float deltaTime);
		void onRender();
		inline graphics::FPSCamera* getCamera()const { return m_Camera; }
		inline graphics::Renderer* getRenderer() const{ return m_Renderer; }
	private:
		void init();
	};
}