#pragma once

#include <string>
#include <GLUI/GLUI.h>

namespace glui {
	typedef struct Color {
		float r, g, b;
	} Color;

	class Font {
	public:
		Font(std::string path, float a_size);

		void* chars;
		float size;
	};

	typedef struct Rectangle {
		float x, y, w, h;
	} Rectangle;

	typedef struct TextStyle {
		float size;
		Font* font;
		Color color;
	};
}
