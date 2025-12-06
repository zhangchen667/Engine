#pragma once
#include<GLFW\glfw3.h>
#include<gl/GL.h>
namespace myarcane {

	class Timer {
	private:
		double startTime;
	public:
		Timer();
		void reset();
		inline double elapsed()const {
			return glfwGetTime() - startTime;
		};
	};
}