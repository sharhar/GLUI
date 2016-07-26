#include <GLUI/Window.h>
#include <GLUI/Input.h>
#include <iostream>
#include <GLFW/glfw3.h>

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

	Window::Window(const char* title, int width, int height) {
		m_width = width;
		m_height = height;

		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mousePosCallback);
		m_window = (void*)window;
	}

	void Window::poll() {
		glfwPollEvents();
	}

	bool Window::isOpen() {
		return !glfwWindowShouldClose((GLFWwindow*) m_window);
	}

	void Window::destroy() {
		glfwDestroyWindow((GLFWwindow*) m_window);
	}
}