#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>

namespace glui {
	typedef void(*func_callBack)(void);

	typedef struct BasicButtonDescriptor {
		TextStyle style;
		func_callBack callBack;
	} BasicButtonDescriptor;

	typedef struct ButtonDescriptor {
		BasicButtonDescriptor basic;
		Color ntr, ntl, nbr, nbl;
		Color htr, htl, hbr, hbl;
		Color dtr, dtl, dbr, dbl;
		Color outColor;
		float outThick;
		bool out;
	} ButtonDescriptor;

	class Button : public GLUIObject {
	private:
		std::string m_text;
		ButtonDescriptor m_descriptor;
		bool m_prevDown;
		bool m_down;
		bool m_hovering;
		bool m_basic;
	public:
		Button(Rectangle bounds, Layout* layout, std::string text, BasicButtonDescriptor desc);
		Button(Rectangle bounds, Layout* layout, std::string text, ButtonDescriptor desc);

		void poll() override;
		void render() override;
	};
}