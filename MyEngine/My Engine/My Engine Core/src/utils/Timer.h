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
		double elapsed();
	};
}