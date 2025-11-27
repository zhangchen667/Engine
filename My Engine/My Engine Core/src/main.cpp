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
#include"graphics/model.h"
#include"terrain/Terrain.h"
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

	/*myarcane::graphics::Shader shader("src/shaders/basic.vert", "src/shaders/multipleLight.frag");
	myarcane::graphics::Shader lampShader("src/shaders/basic.vert", "src/shaders/lightCube.frag");*/
	myarcane::graphics::Shader shader("src/shaders/basic.vert", "src/shaders/terrain.frag");

	stbi_set_flip_vertically_on_load(true);//翻转y轴

	
	//顶点数据
	GLfloat vertices[] = {
		// Positions         // Normals           // Texture Coords (UV Mapping)
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
	glm::vec3 cubePositions[] = {//立方体位置
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glm::vec3 pointLightPositions[] = {
		glm::vec3(3.0f,  0.2f,  -1.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//GLuint VBO, VAO, lightVAO;//VAO顶点属性的配置状态，VBO存放顶点数据
	//glGenVertexArrays(1, &VAO);
	//glGenVertexArrays(1, &lightVAO);
	//glGenBuffers(1, &VBO);

	//
	//
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////顶点位置索引
	//glBindVertexArray(VAO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8* sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	////颜色索引
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);
	////取消绑定VAO
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// 光源绘制
	//glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0); //unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	////加载纹理
	//GLuint diffuseMap, specularMap,emissionMap;
	//glGenTextures(1, &diffuseMap);
	//glGenTextures(1, &specularMap);
	//glGenTextures(1, &emissionMap);
	//int width, height, nrChannels;
	////漫反射贴图
	//unsigned char* image = stbi_load("res/container2.png", &width, &height, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, diffuseMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(image);
	////镜面反射贴图
	//image = stbi_load("res/container2_specular.png", &width, &height, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, specularMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(image);
	////自发光贴图
	//image = stbi_load("res//container2_emission.png", &width, &height, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, emissionMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D); // Generate mip maps for what is currently bounded to GL_TEXTURE_2D
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(image);

	//shader.enable();
	/*shader.setUniform1i("material.diffuse", 0);
	shader.setUniform1i("material.specular", 1);
	shader.setUniform1i("material.emission", 2);*/
	
	//绑定纹理(不需要在循环中)
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);*/
	
	//加载复杂模型
	glm::vec3 temp = glm::vec3(0, 0, 0);
	myarcane::terrain::Terrain terrain(temp);
	
	shader.enable();
	std::string test = "res/3D_Models/Crysis/nanosuit.obj";
	myarcane::graphics::Model nanosuitModel(test.c_str());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//（多边形的面，渲染模式（默认，点，线））
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
		glClearColor(0.26f, 0.95f, 0.9f, 1.0f);
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
		/*shader.enable();
		glm::vec3 cameraPosition = camera.getPosition();
		shader.setUniform3f("viewPos", glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));

		shader.setUniform1f("material.shininess", 32.0f);*/
		//
		//// directional light
		//shader.setUniform3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		//shader.setUniform3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		//shader.setUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		//// point lights
		//shader.setUniform3f("pointLights[0].position", pointLightPositions[0]);
		//shader.setUniform3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//shader.setUniform3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("pointLights[0].constant", 1.0f);
		//shader.setUniform1f("pointLights[0].linear", 0.09);
		//shader.setUniform1f("pointLights[0].quadratic", 0.032);

		//shader.setUniform3f("pointLights[0].position", pointLightPositions[0]);
		//shader.setUniform3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//shader.setUniform3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("pointLights[0].constant", 1.0f);
		//shader.setUniform1f("pointLights[0].linear", 0.09);
		//shader.setUniform1f("pointLights[0].quadratic", 0.032);
		//// point light 2
		//shader.setUniform3f("pointLights[1].position", pointLightPositions[1]);
		//shader.setUniform3f("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//shader.setUniform3f("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("pointLights[1].constant", 1.0f);
		//shader.setUniform1f("pointLights[1].linear", 0.09);
		//shader.setUniform1f("pointLights[1].quadratic", 0.032);
		//// point light 3
		//shader.setUniform3f("pointLights[2].position", pointLightPositions[2]);
		//shader.setUniform3f("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//shader.setUniform3f("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("pointLights[2].constant", 1.0f);
		//shader.setUniform1f("pointLights[2].linear", 0.09);
		//shader.setUniform1f("pointLights[2].quadratic", 0.032);
		//// point light 4
		//shader.setUniform3f("pointLights[3].position", pointLightPositions[3]);
		//shader.setUniform3f("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader.setUniform3f("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//shader.setUniform3f("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("pointLights[3].constant", 1.0f);
		//shader.setUniform1f("pointLights[3].linear", 0.09);
		//shader.setUniform1f("pointLights[3].quadratic", 0.032);
		//// spotLight
		//shader.setUniform3f("spotLight.position", camera.getPosition());
		//shader.setUniform3f("spotLight.direction", camera.getFront());
		//shader.setUniform3f("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		//shader.setUniform3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//shader.setUniform1f("spotLight.constant", 1.0f);
		//shader.setUniform1f("spotLight.linear", 0.09);
		//shader.setUniform1f("spotLight.quadratic", 0.032);
		//shader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//shader.setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//
		glm::mat4 view;
		view = camera.getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getFov()), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 500.0f);
	
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);
		shader.setUniform1f("time", glfwGetTime());
		

		//glBindVertexArray(VAO);
		////绘制多个立方体
		//for (unsigned int i = 0; i < 10; i++) {
		//	glm::mat4 model=glm::mat4(1.0);
		//	model = glm::translate(model, cubePositions[i]);
		//	model = glm::rotate(model, glm::radians(20.0f * (i + (float)glfwGetTime())), glm::vec3(0.3f, 0.5f, 1.0f));
		//	shader.setUniformMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);

		glm::mat4 model=glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		shader.setUniformMat4("model", model);
		nanosuitModel.Draw(shader);
		//

		//glBindVertexArray(lightVAO);
		//lampShader.enable();
		//lampShader.setUniformMat4("view", view);
		//lampShader.setUniformMat4("projection", projection);
		//// LightCube
		//for (unsigned int i = 0; i < 4; ++i) {
		//	glm::mat4 model = glm::mat4(1.0);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//	lampShader.setUniformMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);
		
		terrain.Draw(shader);
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

	//释放资源
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);*/
	return 0;
}


