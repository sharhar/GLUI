#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Input.h>

namespace glui {

	typedef struct TextBoxDescriptor {
		TextStyle style;
		Color boxColor;
		Color outlineColor;
		Color cursorColor;
		float cursorWidth;
		float outLineWidth;
	} TextBoxDescriptor;

	class TextBox : public GLUIObject {
	private:
		TextBoxDescriptor m_desc;
		float cursorXOff;

		bool m_prevDown;
		bool m_writting;
	public:
		std::string m_text;
	
		TextBox(Rectangle bounds, Layout* layout, TextBoxDescriptor desc);

		void poll() override;
		void render() override;
	};
}