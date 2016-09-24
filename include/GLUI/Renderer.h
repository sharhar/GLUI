#pragma once

#include <GLUI/Window.h>
#include <GLUI/Utils.h>
#include <string>

namespace glui {
	class Renderer {
	private:
		static unsigned int glInitList;
	public:
		static void init(Window* window);
		static void reinit();
		static void clear(Color color);
		static void drawString(const std::string& text, float posx, float posy, float scale, Font* font, Color* color);
		static void drawString(const std::string& text, int num, float posx, float posy, float scale, Font* font, Color* color);
	};
}