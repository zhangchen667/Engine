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
//#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"
//myarcane::graphics::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
myarcane::graphics::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
myarcane::graphics::Window window("My Engine",1366,768);

GLfloat yaw = -90.0f;//偏航角
GLfloat pitch = 0.0f;//俯仰角
//xOffset
int main() {
	myarcane::Logger log;
	log.info("Shader Initialization", "Shader successfully initialized");

	glEnable(GL_DEPTH_TEST);

	myarcane::graphics::Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	myarcane::graphics::Shader lampShader("src/shaders/basic.vert", "src/shaders/lightCube.frag");

	stbi_set_flip_vertically_on_load(true);//翻转y轴

	
	//顶点数据
	GLfloat vertices[] = {
		// Positions         // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	GLuint VBO, VAO, lightVAO;//VAO顶点属性的配置状态，VBO存放顶点数据
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//顶点位置索引
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8* sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//颜色索引
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//取消绑定VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 光源绘制
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 光源位置
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	//加载纹理
	GLuint diffuseMap, specularMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	int width, height, nrChannels;
	//漫反射贴图
	unsigned char* image = stbi_load("res/container2.png", &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
	//镜面反射贴图
	image = stbi_load("res/container2_specular.png", &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	shader.enable();
	shader.setUniform1i("material.diffuse", 0);
	shader.setUniform1i("material.specular", 1);

	int frames = 0;
	//temp rotation timer
	myarcane::Timer count;
	myarcane::Timer timer;
	myarcane::Time time;

	bool firstMove= true;
	GLfloat lastX = window.getWidth();
	GLfloat lastY = window.getHeight();//屏幕极限

	//渲染循环
	while (!window.closed()) {
		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
		window.clear();
		time.update();
		// 检测鼠标移动
		if (firstMove && (lastX != window.getMouseX() || lastY != window.getMouseY())) {
			lastX = window.getMouseX();
			lastY = window.getMouseY();
			firstMove = false;
		}
		camera.processMouseMovement(window.getMouseX() - lastX, lastY - window.getMouseY(), true);
		lastX = window.getMouseX();
		lastY = window.getMouseY();

		if (window.isKeyPressed(GLFW_KEY_W))
			camera.processKeyboard(myarcane::graphics::FORWARD, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_S))
			camera.processKeyboard(myarcane::graphics::BACKWARD, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_A))
			camera.processKeyboard(myarcane::graphics::LEFT, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_D))
			camera.processKeyboard(myarcane::graphics::RIGHT, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_SPACE))
			camera.processKeyboard(myarcane::graphics::UPWARDS, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			camera.processKeyboard(myarcane::graphics::DOWNWARDS, time.getDeltaTime());
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
			window.setclosed();
		camera.processMouseScroll(window.getScrollY()*6);
		window.resetScroll();

		// 光源变量的设置
		shader.enable();
		lightPos.x = sin(glfwGetTime()) * 2.0f;
		lightPos.y = cos(glfwGetTime()) * 1.5f;
		lightPos.z = -2.0f;
		
		glm::vec3 cameraPosition = camera.getPosition();
		shader.setUniform3f("viewPos", glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));

		shader.setUniform1f("material.shininess", 32.0f);

		shader.setUniform3f("light.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
		shader.setUniform3f("light.ambient", glm::vec3(0.15f,0.15f,0.15f));
		shader.setUniform3f("light.diffuse", glm::vec3(0.6f,0.6f,0.6f));
		shader.setUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -7.0f));//平移矩阵
		model = glm::rotate(model, (GLfloat)count.elapsed(), glm::vec3(1.0f, 0.5f, 0.2f));//旋转矩阵（随时间）
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glm::mat4 view;
		view = camera.getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getFov()), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
		shader.setUniformMat4("model", model);
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);

		// Bind Diffuse Map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// Bind Specular Map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);//36个顶点
		glBindVertexArray(0);

		//光源的绘制
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		lampShader.enable();
		lampShader.setUniformMat4("model", model);
		lampShader.setUniformMat4("view", view);
		lampShader.setUniformMat4("projection", projection);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		window.update();
		if (timer.elapsed() >= 1) {
			std::cout << "FPS: " << frames << std::endl;
			frames = 0;
			timer.reset();
		}
		else {
			frames++;
		}
	}
	return 0;
}


