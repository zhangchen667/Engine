#include"FPSCamera.h"
#include<iostream>
namespace myarcane {
	namespace graphics {
		FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
			:m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Fov(FOV)
		{
			m_Position = position;
			m_WorldUp = up;
			m_Up = up;
			m_Yaw = yaw;
			m_Pitch = pitch;
			updateCameraVectors();
		}
		FPSCamera::FPSCamera(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat yaw=YAW, GLfloat pitch=PITCH)
			: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Fov(FOV)
		{
			m_Position = glm::vec3(xPos, yPos, zPos);
			m_WorldUp= glm::vec3(xUp, yUp, zUp);
			m_Yaw = yaw;
			m_Pitch = pitch;
			updateCameraVectors();
		}

		glm::mat4 FPSCamera::getViewMatrix() {
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}
		void FPSCamera::processKeyboard(Camera_Movement direction, GLfloat deltaTime) {
			GLfloat velocity = m_MovementSpeed * deltaTime;
			switch (direction)
			{
			case FORWARD:
				m_Position += m_Front * velocity;
				break;
			case BACKWARD:
				m_Position -= m_Front * velocity;
				break;
			case LEFT:
				m_Position -= m_Right * velocity;
				break;
			case RIGHT:
				m_Position += m_Right * velocity;
				break;	
			case UPWARDS:
				m_Position += m_WorldUp * velocity;
				break;
			case DOWNWARDS:
				m_Position -= m_WorldUp * velocity;
				break;
			}
		}
		void FPSCamera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
			xOffset *= m_MouseSensitivity;
			yOffset *= m_MouseSensitivity;

			m_Yaw += xOffset;
			m_Pitch += yOffset;

			//限制俯仰角
			if (constrainPitch) {
				if (m_Pitch > 89.0f) {
					m_Pitch = 89.0f;
				}
				else if (m_Pitch < -89.0f) {
					m_Pitch = -89.0f;
				}
			}

			updateCameraVectors();
		}
		void FPSCamera::processMouseScroll(GLfloat yOffset) {
			if (m_Fov >= 1.0f && m_Fov <= FOV) {
				m_Fov -= yOffset;
			}
			if (m_Fov < 1.0f) {
				m_Fov = 1.0f;
			}
			else if (m_Fov > FOV) {
				m_Fov = FOV;
			}
		}
		void FPSCamera::updateCameraVectors() {
			glm::vec3 front;
			front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			front.y = sin(glm::radians(m_Pitch));
			front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front = glm::normalize(front);

			//重新计算右向量和上向量
			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		}
	}
}