#include "Timer.h"

namespace myarcane {

	Timer::Timer() {
		startTime = glfwGetTime();
	}

	void Timer::reset() {
		startTime = glfwGetTime();
	}

	double Timer::elapsed() {
		return glfwGetTime() - startTime;
	}

};