#include<iostream>
#include"graphics/Window.h"
#include"graphics/camera/FPSCamera.h"

#include "graphics\Shader.h"
#include <cmath>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"utils/Timer.h"
//#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"
//myarcane::graphics::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
void move();
void checkZoom(float& fov);
glm::vec3 cameraPos(0.0f, 0.0f, 5.0f); glm::vec3 cameraFront(0.0f, 0.0f, -1.0f); glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
myarcane::graphics::Window window("My Engine",1366,768);
GLfloat lastX = window.getWidth();
GLfloat lastY = window.getHeight();//窗口极限
GLfloat yaw = -90.0f;//偏航角
GLfloat pitch = 0.0f;//俯仰角
//xOffset
int main() {
	glEnable(GL_DEPTH_TEST);
	myarcane::graphics::Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();
	stbi_set_flip_vertically_on_load(true);//翻转y轴

	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	
	int width, height,nrComponents;
	glBindTexture(GL_TEXTURE_2D, texture1);
	unsigned char* image = stbi_load("res/container.jpg", &width, &height, &nrComponents, 0);
	if (image) {
		GLenum internalFormat, format;
		if (nrComponents == 1) {
			internalFormat = GL_RED;
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			internalFormat = GL_RGB;
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			internalFormat = GL_RGBA;  // 适配RGBA通道
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(image);
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	unsigned char* image2 = stbi_load("res/awesomeface.png", &width, &height, &nrComponents, 0);
	if (image2) {
		GLenum internalFormat, format;
		if (nrComponents == 1) {
			internalFormat = GL_RED;
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			internalFormat = GL_RGB;
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			internalFormat = GL_RGBA;  // 适配RGBA通道
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(image2);
	}
	else {
		std::cout << "Failed to load texture2" << std::endl;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 检查ourTexture1的位置
	GLint loc1 = glGetUniformLocation(shader.getShaderID(), "texture1");
	if (loc1 == -1) {
		std::cout << "Uniform 'texture1' not found in shader!" << std::endl;
	}
	glUniform1i(loc1, 0);

	// 检查ourTexture2的位置
	GLint loc2 = glGetUniformLocation(shader.getShaderID(), "texture2");
	if (loc2 == -1) {
		std::cout << "Uniform 'texture2' not found in shader!" << std::endl;
	}
	glUniform1i(loc2, 1);
	//顶点数据
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,	     1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,	     0.0f, 1.0f,	1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,	     0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,	     1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,	     1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,	     1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,	     0.0f, 1.0f,	1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,	     0.0f, 0.0f,	1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f,	     1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,	     0.0f, 0.0f,	1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,	     1.0f, 0.0f,	1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f,	     1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,	     1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,	     0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
	};

	GLuint VBO, VAO, EBO;//VAO顶点属性的配置状态，VBO存放顶点数据
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//顶点位置索引
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//颜色索引
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//纹理索引
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//取消绑定VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	int frames = 0;
	//temp rotation timer
	myarcane::Timer count;
	myarcane::Timer timer;

	float fov = 45.0f;
	lastX = window.getMouseX();
	lastY = window.getMouseY();
	shader.enable();
	glUniform1i(glGetUniformLocation(shader.getShaderID(), "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader.getShaderID(), "texture2"), 1);
	//渲染循环
	while (!window.closed()) {
		window.clear();
		
		GLfloat xOffset = window.getMouseX() - lastX;
		GLfloat yOffset = lastY - window.getMouseY(); // Reversed since y is top to bottom
		lastX = window.getMouseX();
		lastY = window.getMouseY();
		GLfloat sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		yaw += xOffset;
		pitch += yOffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		checkZoom(fov);
		move();
		glm::mat4 model(1);
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
		shader.setUniformMat4("model", model);
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);//36个顶点
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


//通过鼠标进行视野角度fov的缩放
float origFov;
bool first = true;
void checkZoom(float& fov) {
	if (first) {
		origFov = fov;
		first = false;
	}
	if (window.getScrollX() == 1 && fov > 1.0f) {
		fov--;
		window.resetScroll();
	}
	if (window.getScrollY() == -1 && fov < origFov) {
		fov++;
		window.resetScroll();
	}
}
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
void move() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat cameraSpeed = 2.0f * deltaTime;
	if (window.isKeyPressed(GLFW_KEY_W)) {
		cameraPos += cameraFront * cameraSpeed;
	}
	if (window.isKeyPressed(GLFW_KEY_S)) {
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (window.isKeyPressed(GLFW_KEY_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (window.isKeyPressed(GLFW_KEY_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

