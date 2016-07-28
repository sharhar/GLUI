#pragma once

namespace glui{
	class Window {
	private:
		void* m_window;
		int m_width;
		int m_height;
	public:
		Window(const char* title, int width, int height);
		void poll();
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		bool isOpen();
		void swap();
		void destroy();

		void* getGLFWwindow();
	};
}