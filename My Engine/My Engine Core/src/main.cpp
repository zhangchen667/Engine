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
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"
#include"graphics/model.h"
#include"terrain/Terrain.h"
#include"Scene3D.h"
#include"platform/OpenGL/Framebuffer.h"
#include"graphics/MeshFactory.h"
#include <random>

void renderQuad(GLuint quadVAO) {
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

int main(){
	myarcane::graphics::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f,1.0f,0.0f),-90,0.0f);
	myarcane::graphics::Window window("MyArcane Engine",1366,768);
	myarcane::Scene3D scene(&camera, &window);

	// --- Framebuffers ---
	int width = window.getWidth();
	int height = window.getHeight();

	// MSAA FBO for scene render
	myarcane::opengl::Framebuffer framebuffer(width, height);
	// Blit FBO with depth texture (SSAO needs to sample depth)
	myarcane::opengl::Framebuffer blitFramebuffer(width, height, false, true);

	// --- SSAO FBOs ---
	GLuint ssaoFBO, ssaoColorBuffer;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

	GLuint ssaoBlurFBO, ssaoBlurColorBuffer;
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoBlurColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoBlurColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// --- SSAO kernel (64 hemisphere samples) ---
	std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
	std::default_random_engine rng;
	std::vector<glm::vec3> ssaoKernel(64);
	for (unsigned int i = 0; i < 64; ++i) {
		glm::vec3 sample(randomFloats(rng) * 2.0f - 1.0f, randomFloats(rng) * 2.0f - 1.0f, randomFloats(rng));
		sample = glm::normalize(sample) * randomFloats(rng);
		float scale = float(i) / 64.0f;
		scale = 0.1f + scale * scale * 0.9f;
		sample *= scale;
		ssaoKernel[i] = sample;
	}

	// --- SSAO noise texture (4x4 random rotation vectors) ---
	GLuint noiseTexture;
	std::vector<glm::vec3> ssaoNoise(16);
	for (unsigned int i = 0; i < 16; ++i) {
		ssaoNoise[i] = glm::vec3(randomFloats(rng) * 2.0f - 1.0f, randomFloats(rng) * 2.0f - 1.0f, 0.0f);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// --- Post-processing screen quad (standalone VAO) ---
	GLuint quadVAO, quadVBO;
	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// --- Shaders ---
	myarcane::graphics::Shader ssaoShader("src/shaders/postProcess.vert", "src/shaders/ssao.frag");
	myarcane::graphics::Shader ssaoBlurShader("src/shaders/postProcess.vert", "src/shaders/ssaoBlur.frag");
	myarcane::graphics::Shader fxaaShader("src/shaders/postProcess.vert", "src/shaders/fxaa.frag");

	// Set SSAO kernel uniforms once
	ssaoShader.enable();
	for (unsigned int i = 0; i < 64; ++i) {
		ssaoShader.setUniform3f(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
	}
	ssaoShader.setUniform2f("noiseScale", glm::vec2(width / 4.0f, height / 4.0f));

	myarcane::Timer fpsTimer;
	int frames = 0;
	myarcane::Time deltaTime;
	bool firstMove = true;
	GLfloat lastX = window.getWidth();
	GLfloat lastY = window.getHeight();

	// --- Render loop ---
	while (!window.closed()) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		window.clear();
		deltaTime.update();

		if (firstMove && (lastX != window.getMouseX() || lastY != window.getMouseY())) {
			lastX = window.getMouseX();
			lastY = window.getMouseY();
			firstMove = false;
		}
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
		camera.processMouseScroll(window.getScrollY() * 6);
		window.resetScroll();

		glm::mat4 projMatrix = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 1000.0f);
		glm::mat4 invProjMatrix = glm::inverse(projMatrix);

		// ---- Pass 1: Scene render to MSAA FBO ----
		framebuffer.bind();
		window.clear();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.onUpdate(deltaTime.getDeltaTime());
		scene.onRender();

		// ---- Pass 2: MSAA resolve (blit color + depth) ----
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.getFramebuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, blitFramebuffer.getFramebuffer());
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		// ---- Pass 3: SSAO ----
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		ssaoShader.enable();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blitFramebuffer.getDepthTexture());
		ssaoShader.setUniform1i("depthMap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		ssaoShader.setUniform1i("noiseTexture", 1);
		ssaoShader.setUniformMat4("projection", projMatrix);
		ssaoShader.setUniformMat4("invProjection", invProjMatrix);
		renderQuad(quadVAO);

		// ---- Pass 4: SSAO blur ----
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		ssaoBlurShader.enable();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		ssaoBlurShader.setUniform1i("ssaoInput", 0);
		renderQuad(quadVAO);

		// ---- Pass 5: FXAA + SSAO apply -> screen ----
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fxaaShader.enable();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blitFramebuffer.getColourBufferTexture());
		fxaaShader.setUniform1i("sceneColor", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssaoBlurColorBuffer);
		fxaaShader.setUniform1i("ssaoMap", 1);
		fxaaShader.setUniform2f("sampleOffset", glm::vec2(1.0f / width, 1.0f / height));
		renderQuad(quadVAO);

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
