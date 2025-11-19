#define V_SYNC 1	//1启动垂直同步
#define FULLSCREEN_MODE 0//1全屏模式
#define SHOW_MOUSE 0//1显示鼠标光标
#include"Window.h"
namespace myarcane {
	namespace graphics {
		//回调函数功能声明
		static void error_callback(int error, const char* description);
		static void window_resize(GLFWwindow* window, int width, int height);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		Window::Window(const char* title, int width, int height) {
			m_Title = title;
			m_Width = width;
			m_Height = height;

			if (!init()) {
				glfwDestroyWindow(m_Window);//销毁窗口
				glfwTerminate();//释放资源
			}
		}
		Window::~Window() {
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
		bool Window::init() {
			if (!glfwInit()) {
				std::cout << "GLFW Failed To Initialize" << std::endl;
			}

			// Create the window
			if (FULLSCREEN_MODE) {//全屏
				setFullscreenResolution();//获得主显示屏分辨率
				m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, glfwGetPrimaryMonitor(), NULL);
			}
			else {
				m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
			}

			if (!m_Window) {
				std::cout << "GLFW Window Couldn't Be Created" << std::endl;
			}

			// Setup the mouse settings
			if (!SHOW_MOUSE)//不显示鼠标光标时
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//禁用鼠标光标

			// Set up contexts and callbacks
			glfwMakeContextCurrent(m_Window);//将窗口设置为当前opengl上下文（渲染目标）
			glfwSetWindowUserPointer(m_Window, this);//关联窗口与当前window实例
			//设置各种回调事件
			glfwSetErrorCallback(error_callback);
			glfwSetWindowSizeCallback(m_Window, window_resize);
			glfwSetKeyCallback(m_Window, key_callback);
			glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, cursor_position_callback);
			glfwSetScrollCallback(m_Window, scroll_callback);


			// Check to see if v-sync was enabled and act accordingly
			if (V_SYNC) {//垂直同步
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}


			// Initialize GLEW (allows us to use newer versions of OpenGL)
			if (glewInit() != GLEW_OK) {
				std::cout << "Could not Initialize GLEW" << std::endl;
				return 0;
			}
			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			// Everything was successful so return true
			return 1;
		}
		void Window::update() {
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				std::cout << "OpenGL Error: " << error << std::endl;
			}
			glfwSwapBuffers(m_Window);//交换opengl渲染的前后缓冲区
			glfwPollEvents();//检测并处理所有待处理的输入事件
		}
		void Window::clear()const {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		bool Window::closed() const {
			return glfwWindowShouldClose(m_Window);
		}
		void Window::setclosed() {
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}
		void Window::setFullscreenResolution() {
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());//查询主显示器模式（里边monitor返回句柄）
			m_Width = mode->width;
			m_Height = mode->height;
		}

		bool Window::isKeyPressed(unsigned int code)const {
			if (code >= MAX_KEYS) {
				return false;
			}
			else {
				return m_Keys[code];
			}
		}
		bool Window::isMouseButtonPressed(unsigned int code) const {
			if (code >= MAX_BUTTONS) {
				//TODO: LOG THIS
				return false;
			}
			else {
				return m_Buttons[code];
			}
		}
		//回调函数功能实现
		static void error_callback(int error, const char* description) {
			std::cout << "Error:" << std::endl << description << std::endl;
		}
		static void window_resize(GLFWwindow* window, int width, int height) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);// 从GLFW窗口中获取用户自定义指针，并转换为Window*类型
			win->m_Width = width;
			win->m_Height = height;
			glViewport(0, 0, win->m_Width, win->m_Height);
		}
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_Keys[key] = action != GLFW_RELEASE;//按下或重复按下都视为按下状态
		}
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_Buttons[button] = action != GLFW_RELEASE;
		}

		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->mx = xpos;
			win->my = ypos;
		}

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->scrollX = xoffset;
			win->scrollY = yoffset;
		}
	}
}