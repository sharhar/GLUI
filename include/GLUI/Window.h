#pragma once

#include <GLUI/Utils.h>

struct GLFWwindow;
struct GLFWimage;

namespace glui{

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
		void poll();
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		int getX() { return m_xpos; }
		int getY() { return m_ypos; }
		bool isOpen();
		void swap();
		void destroy();
		static int popup(PopupDescriptor desc, Theme theme);

		void* getGLFWwindow();

		friend static void focusCallback(GLFWwindow* window, int focused);
		friend static void posCallback(GLFWwindow* window, int xpos, int ypos);
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