#include <Window.h>
#include <iostream>
#include <GLFW/glfw3.h>

namespace glui {
	Window::Window(const char* title, int width, int height) {
		m_width = width;
		m_height = height;

		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
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