#pragma once
#include"../Defs.h";
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"../utils/Logger.h"
namespace myarcane {
	namespace graphics {
#define MAX_KEYS 1024
#define MAX_BUTTONS 32
		class Window {
		private:
			const char* m_Title;
			int m_Width, m_Height;
			GLFWwindow* m_Window;
			

			bool m_Keys[MAX_KEYS];	
			bool m_Buttons[MAX_BUTTONS];
			double mx, my;
			double scrollX, scrollY;
		public:
			Window(const char* title, int width, int height);//构造
			~Window();
			void update();
			void clear() const;
			bool closed() const;
			void setclosed();
			bool isKeyPressed(unsigned int Keycode)const;
			bool isMouseButtonPressed(unsigned int code) const;

			inline double getMouseX() const{ return mx; };
			inline double getMouseY() const{ return my; };
			inline double getScrollX() const{ return scrollX; };
			inline double getScrollY() const{ return scrollY; };
			inline void resetScroll() { scrollX = 0; scrollY = 0; };
			inline void getMousePosition(double& x, double& y)const { x = mx; y = my; };
			inline int getWidth()const { return m_Width; };
			inline int getHeight()const { return m_Height; };
		private:
			bool init();
			void setFullscreenResolution();//设置视口分辨率
			static friend void error_callback(int error, const char* description);
			static friend void window_resize(GLFWwindow* window, int width, int height);
			static friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
			friend static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		};
	}
}