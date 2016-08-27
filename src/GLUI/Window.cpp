#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::callbacks::key(key, action);
	}

	static void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
		input::callbacks::mousePosition((int) xpos, (int) ypos);
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		input::callbacks::mouseButton(button, action);
	}

	static void textCallBack(GLFWwindow* window, unsigned int codepoint) {
		input::callbacks::text(codepoint);
	}

	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		input::callbacks::mouseScroll(yoffset);
	}

	Window::Window(const char* title, int width, int height) {
		m_width = width;
		m_height = height;

		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);

		glfwSwapInterval(1);

		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mousePosCallback);
		glfwSetCharCallback(window, textCallBack);
		glfwSetScrollCallback(window, mouseScrollCallback);
		m_window = window;
	}

	void Window::poll() {
		glfwPollEvents();

		GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			if(err == 1282) {//temprorary solution to GLError: 1282 that doesnt seem to do anything
				continue;
			}
			std::cout << "GLError: " << err << "\n";
		}
	}

	void Window::swap() {
		glfwSwapBuffers(m_window);
	}

	bool Window::isOpen() {
		return !glfwWindowShouldClose(m_window);
	}

	void Window::destroy() {
		glfwDestroyWindow(m_window);
	}

	void* Window::getGLFWwindow() {
		return m_window;
	}
}