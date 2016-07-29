#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>
#include <GLUI/Input.h>

namespace glui {

	typedef struct TextBoxDescriptor {
		TextStyle style;
		Layout* layout;
		Color boxColor;
		Color outlineColor;
		Color cursorColor;
		float cursorWidth;
		float outLineWidth;
	} TextBoxDescriptor;

	class TextBox {
	private:
		TextBoxDescriptor m_desc;
		Rectangle m_rect;
		std::string m_text;
		float cursorXOff;

		bool m_prevDown;
		bool m_writting;
	public:
		TextBox(Rectangle rect, TextBoxDescriptor desc);

		void poll();
		void render();
	};
}