#pragma once

#include <GLUI/Utils.h>
#include <string>

namespace glui {
	class Renderer {
	public:
		static void init();
		static void clear();
		static void drawString(const std::string& text, float posx, float posy, float scale, Font* font, Color* color);
	};
}