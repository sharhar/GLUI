#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>
#include <string>

namespace glui {

	typedef struct CheckBoxDescriptor {
		TextStyle textStyle;
		Color boxColor;
		float outLineThickness;
		Color outlineColor;
		Color checkColor;
	} CheckBoxDescriptor;

	class CheckBox : public GLUIObject {
	private:
		std::string m_text;
		CheckBoxDescriptor m_desc;

		bool m_checked;
		bool m_prevDown;
	public:
		CheckBox(Rectangle bounds, Layout* layout, std::string text, CheckBoxDescriptor desc);
		void poll() override;
		void render() override;
		bool checked() { return m_checked; }
	};
}