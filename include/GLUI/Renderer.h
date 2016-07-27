#pragma once

#include <GLUI/Utils.h>
#include <string>

namespace glui {
	class Renderer {
	public:
		static void render(const std::string& text, int posx, int posy, Font* font, Color* color);
	};
}