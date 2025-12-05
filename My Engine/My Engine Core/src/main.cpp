#include<iostream>
#include"graphics/Window.h"
#include"graphics/camera/FPSCamera.h"
#include"utils/Time.h"
#include "graphics\Shader.h"
#include <cmath>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"utils/Timer.h"
#include"utils\Logger.h"
//#include<ft2build.h>
//#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"
#include"graphics/model.h"
#include"terrain/Terrain.h"
//#include FT_FREETYPE_H 
//#include<freetype-gl/freetype-gl.h>
#include"Scene3D.h"
int main(){
	myarcane::graphics::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f,1.0f,0.0f),-90,0.0f);
	myarcane::graphics::Window window("MyArcane Engine",1366,768);
	myarcane::Scene3D scene(&camera, &window);
	
	myarcane::Timer fpsTimer;//FPS计时器,用于计算每秒帧数
	int frames = 0;
	myarcane::Time deltaTime;//帧时间计算,用于平滑移动

	bool firstMove= true;
	GLfloat lastX = window.getWidth();
	GLfloat lastY = window.getHeight();//屏幕极限

	//渲染循环
	while (!window.closed()) {
		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
		window.clear();
		deltaTime.update();//更新帧时间
		// 检测鼠标移动
		if (firstMove && (lastX != window.getMouseX() || lastY != window.getMouseY())) {
			lastX = window.getMouseX();
			lastY = window.getMouseY();
			firstMove = false;
		}
		//摄像机控制
		camera.processMouseMovement(window.getMouseX() - lastX, lastY - window.getMouseY(), true);
		lastX = window.getMouseX();
		lastY = window.getMouseY();

		if (window.isKeyPressed(GLFW_KEY_W))
			camera.processKeyboard(myarcane::graphics::FORWARD, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_S))
			camera.processKeyboard(myarcane::graphics::BACKWARD, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_A))
			camera.processKeyboard(myarcane::graphics::LEFT, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_D))
			camera.processKeyboard(myarcane::graphics::RIGHT, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_SPACE))
			camera.processKeyboard(myarcane::graphics::UPWARDS, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			camera.processKeyboard(myarcane::graphics::DOWNWARDS, deltaTime.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
			window.setclosed();
		camera.processMouseScroll(window.getScrollY()*6);
		window.resetScroll();

		scene.onUpdate(deltaTime.getDeltaTime());//把场景绘制复杂步骤集合到一起
		scene.onRender();//
		
		
		window.update();
		if (fpsTimer.elapsed() >= 1) {
			std::cout << "FPS: " << frames << std::endl;
			frames = 0;
			fpsTimer.reset();
		}
		else {
			frames++;
		}
	}
	return 0;
}


