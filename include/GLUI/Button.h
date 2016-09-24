#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>
#include <GLUI/GLUIObject.h>
#include <functional>

namespace glui {
	typedef struct ButtonDescriptor {
		TextStyle style;
		std::function<void(void)> callBack;
		float outThick;
		Theme theme;
	} ButtonDescriptor;

	class Button : public GLUIObject {
	private:
		std::string m_text;
		ButtonDescriptor m_desc;
		bool m_prevDown;
		bool m_down;
		bool m_hovering;
	public:
		Button(Rectangle bounds, Layout* layout, std::string text, ButtonDescriptor desc);

		void poll() override;
		void render() override;
	};
}