#pragma once

#include <GLUI/Utils.h>
#include <functional>

struct GLFWwindow;
struct GLFWimage;

namespace glui{

	typedef struct WindowCallbacks {
		std::function<void(int, int)> key;
		std::function<void(unsigned int)> text;
		std::function<void(int, int)> mousePos;
		std::function<void(int, int)> mouseButton;
		std::function<void(float)> mouseScroll;
	} WindowCallbacks;

	typedef struct PopupDescriptor;

	class Window {
	private:
		GLFWwindow* m_window;
		int m_width;
		int m_height;
		int m_xpos;
		int m_ypos;
		int m_focused;
	public:
		Window(const char* title, int width, int height, bool resizeable, int iconNum, GLFWimage* icon);
		Window(GLFWwindow* glfwWin);
		void poll();
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		int getX() { return m_xpos; }
		int getY() { return m_ypos; }
		bool isOpen();
		void swap();
		void destroy();
		WindowCallbacks getCallbacks();
		static int popup(PopupDescriptor desc, Theme theme);

		GLFWwindow* getGLFWwindow();

		friend void focusCallback(GLFWwindow* window, int focused);
		friend void posCallback(GLFWwindow* window, int xpos, int ypos);
	};

	typedef struct PopupDescriptor {
		int width;
		int height;
		const char* title;
		const char* text;
		int btnNum;
		const char** btnText;
		Window* window;
		TextStyle bodyTextStyle;
		TextStyle buttonTextStyle;
	} PopupDescriptor;
}