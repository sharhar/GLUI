#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Input.h>

namespace glui {

	typedef struct TextBoxDescriptor {
		TextStyle style;
		float cursorWidth;
		float outLineWidth;
		Theme theme;
	} TextBoxDescriptor;

	class TextBox : public GLUIObject {
	private:
		TextBoxDescriptor m_desc;
		size_t m_cursorPos;
		size_t m_prevTextSize;

		bool m_prevDown;
		bool m_writting;
		bool m_isActive;

		bool m_renderCursor;
		double m_time;

		unsigned char* m_prevKeys;
	public:
		std::string m_text;
	
		TextBox(Rectangle bounds, Layout* layout, TextBoxDescriptor desc);

		void poll() override;
		void render() override;
	};
}