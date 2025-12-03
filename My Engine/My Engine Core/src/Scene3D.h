#pragma once
#include"graphics/camera/FPSCamera.h"
#include"graphics/renderer/Renderer.h"
namespace myarcane {
	class Scene3D {
	private:
		graphics::Renderer* m_Renderer;
		graphics::FPSCamera* m_Camera;
	public:
		Scene3D(glm::mat4& projectionMatrix);
		~Scene3D();
		//void Add(Entity*e);
		void onUpdate();
		void onRender();
		inline graphics::FPSCamera* getCamera() { return m_Camera; }
		inline graphics::Renderer* getRenderer() { return m_Renderer; }
	};
}