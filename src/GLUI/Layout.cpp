#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	Layout::Layout(Window* window, float width, float height) {
		m_window = window;
		m_width = width;
		m_height = height;
	}

	AbsoluteLayout::AbsoluteLayout(Window* window, float width, float height) : Layout(window, width, height) {
		
	}

	float AbsoluteLayout::getScaleX() {
		return 1;
	}
	
	float AbsoluteLayout::getScaleY() {
		return 1;
	}
}