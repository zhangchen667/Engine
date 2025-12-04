#include"Scene3D.h"
#include<iterator>
#include<iostream>
#include<glm/glm.hpp>

namespace myarcane {
	Scene3D::Scene3D(graphics::FPSCamera* camera, graphics::Window* window)
		:terrainShader("src/shaders/basic.vert", "src/shaders/terrain.frag"), modelShader("src/shaders/basic.vert", "src/shaders/multipleLight.frag"), m_Camera(camera), m_Window(window)
	{
		m_Renderer = new graphics::Renderer();
		glm::vec3 tempworldposition = glm::vec3(0.0f, -20.0f, 0.0f);
		m_Terrain = new terrain::Terrain(tempworldposition);
		init();
	}
	Scene3D::~Scene3D()
	{
		
	}
	void Scene3D::init() {
		//加载模型
		glm::vec3 temp1 = glm::vec3(30.0f, -10.0f, 30.0f);
		glm::vec3 temp2 = glm::vec3(3.0f, 3.0f, 3.0f);
		glm::vec3 temp3 = glm::vec3(0.0f, 1.0f, 0.0f);
		Add(new graphics::Renderable3D(temp1,temp2,temp3,
			0.0f,new myarcane::graphics::Model("res/3D_Models/Crysis/nanosuit.obj")));
		//地形着色器配置
		terrainShader.enable();
		terrainShader.setUniform1f("material.shininess", 32.0f);
		terrainShader.setUniform3f("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.3f));
		terrainShader.setUniform3f("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		terrainShader.setUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		terrainShader.setUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		terrainShader.setUniform3f("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		terrainShader.setUniform3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		terrainShader.setUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		terrainShader.setUniform1f("spotLight.constant", 1.0f);
		terrainShader.setUniform1f("spotLight.linear", 0.022);
		terrainShader.setUniform1f("spotLight.quadratic", 0.0019);
		terrainShader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		terrainShader.setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		terrainShader.setUniform3f("pointLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		terrainShader.setUniform3f("pointLight.diffuse", glm::vec3(0.85f, 0.85f, 0.85f));
		terrainShader.setUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		terrainShader.setUniform1f("pointLight.constant", 1.0f);
		terrainShader.setUniform1f("pointLight.linear", 0.007);
		terrainShader.setUniform1f("pointLight.quadratic", 0.0002);
		//模型着色器配置
		modelShader.enable();
		modelShader.setUniform1f("material.shininess", 32.0f);
		modelShader.setUniform3f("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.3f));
		modelShader.setUniform3f("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		modelShader.setUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		modelShader.setUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		modelShader.setUniform3f("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setUniform3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setUniform1f("spotLight.constant", 1.0f);
		modelShader.setUniform1f("spotLight.linear", 0.022);
		modelShader.setUniform1f("spotLight.quadratic", 0.0019);
		modelShader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		modelShader.setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		modelShader.setUniform3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		modelShader.setUniform3f("pointLights[0].diffuse", glm::vec3(0.85f, 0.85f, 0.85f));
		modelShader.setUniform3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setUniform1f("pointLights[0].constant", 1.0f);
		modelShader.setUniform1f("pointLights[0].linear", 0.007);
		modelShader.setUniform1f("pointLights[0].quadratic", 0.0002);
	}
	void Scene3D::onUpdate(float deltaTime) {
		m_Renderables[0]->setRadianRotation(m_Renderables[0]->getRadianRotation() + deltaTime);//让场景中的第一个三维可渲染对象持续旋转
	}
	void Scene3D::onRender() {
		//模型渲染
		modelShader.enable();
		modelShader.setUniform3f("pointLights[0].position", glm::vec3(30.0f, -10.0f, 30.0f));
		modelShader.setUniform3f("spotLight.position", m_Camera->getPosition());
		modelShader.setUniform3f("spotLight.direction", m_Camera->getFront());
		modelShader.setUniform3f("viewPos", m_Camera->getPosition());
		modelShader.setUniformMat4("view", m_Camera->getViewMatrix());
		modelShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFov()), (float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 1000.0f));

		std::vector<graphics::Renderable3D*>::iterator iter = m_Renderables.begin();
		//遍历场景中的所有三维可渲染对象并提交给渲染器
		while (iter != m_Renderables.end()) {
			m_Renderer->submit((*iter));
			iter++;
		}
		m_Renderer->flush(modelShader);

		//地形渲染
		terrainShader.enable();
		terrainShader.setUniform3f("pointLight.position", glm::vec3(30.0f, -10.0f, 30.0f));
		terrainShader.setUniform3f("spotLight.position", m_Camera->getPosition());
		terrainShader.setUniform3f("spotLight.direction", m_Camera->getFront());
		terrainShader.setUniform3f("viewPos", m_Camera->getPosition());
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_Terrain->getPosition());
		terrainShader.setUniformMat4("model", modelMatrix);
		terrainShader.setUniformMat4("view", m_Camera->getViewMatrix());
		terrainShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFov()), (float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 1000.0f));
		m_Terrain->Draw(terrainShader);
	}
	void Scene3D::Add(graphics::Renderable3D* renderable) {
		m_Renderables.push_back(renderable);
	}
}