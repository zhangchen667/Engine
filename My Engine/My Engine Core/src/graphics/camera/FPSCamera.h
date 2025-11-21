#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace myarcane {
	namespace graphics {
		enum Camera_Movement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UPWARDS,
			DOWNWARDS
		};

		const GLfloat YAW = -90.0f;//水平偏航角(水平)
		const GLfloat PITCH = 0.0f;//俯仰角（上下）
		const GLfloat SPEED = 3.0f;//移动速度
		const GLfloat SENSITIVITY = 0.25f;//鼠标灵敏度
		const GLfloat FOV = 80.0f;//视野范围

		class FPSCamera {
		private:
			//相机位置朝向
			glm::vec3 m_Position;
			glm::vec3 m_Front;
			glm::vec3 m_Up;
			glm::vec3 m_Right;
			glm::vec3 m_WorldUp;
			//相机参数角度
			GLfloat m_Yaw;
			GLfloat m_Pitch;
			//Camera options
			GLfloat m_MovementSpeed;
			GLfloat m_MouseSensitivity;
			GLfloat m_Fov;
		public:
			//向量构造函数
			FPSCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
			//坐标构造函数
			FPSCamera(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat yaw, GLfloat pitch);

			glm::mat4 getViewMatrix();
			void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
			void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch);
			void processMouseScroll(GLfloat yOffset);

			inline GLfloat getYaw() { return m_Yaw; };
			inline GLfloat getPitch() { return m_Pitch; };
			inline GLfloat getFov() { return m_Fov; };
			inline GLfloat getMovementSpeed() { return m_MovementSpeed; };
			inline GLfloat getMouseSensitivity() { return m_MouseSensitivity; };
			inline glm::vec3 getPosition() { return m_Position; };
		private:
			void updateCameraVectors();
		};
	}
}