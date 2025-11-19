#pragma once
#include<GLFW\glfw3.h>
#include<gl/GL.h>
namespace myarcane {
		class Time {
		public:
			GLdouble lastFrame;
			GLdouble delta;
			Time();
			void update();
	};
}