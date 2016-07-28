#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>

namespace glui {
	typedef void(*func_callBack)(void);

	typedef struct BasicButtonDescriptor {
		TextStyle style;
		Layout* layout;
		func_callBack callBack;
	};

	typedef struct ButtonDescriptor {
		BasicButtonDescriptor basic;
		Color ntr, ntl, nbr, nbl;
		Color htr, htl, hbr, hbl;
		Color dtr, dtl, dbr, dbl;
		Color outColor;
		float outThick;
		bool out;
	};

	class Button {
	private:
		Rectangle m_rect;

		std::string m_text;
		ButtonDescriptor m_descriptor;
		bool m_prevDown;
		bool m_down;
		bool m_hovering;
		bool m_basic;
	public:
		Button(Rectangle rect, std::string text, BasicButtonDescriptor desc);

		void poll();
		void render();
	};
}