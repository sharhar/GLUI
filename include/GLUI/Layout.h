#pragma once

#include <GLUI/Window.h>

namespace glui {
	class Layout {
	private:
		Window* m_window;
		float m_width;
		float m_height;
	public:
		Layout(Window* window, float width, float height);

		virtual float getScaleX() = 0;
		virtual float getScaleY() = 0;

		float getWidth() { return m_width; };
		float getHeight() { return m_height; };
	};

	class AbsoluteLayout : public Layout{
	public:
		AbsoluteLayout(Window* window, float width, float height);

		float getScaleX() override;
		float getScaleY() override;
	};
}