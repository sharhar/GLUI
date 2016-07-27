#pragma once

#include <string>
#include <GLUI/GLUI.h>

namespace glui {
	class Color {
	public:
		Color(float a_r, float a_g, float a_b);

		float r, g, b;
	};

	class Font {
	public:
		Font(std::string path);

		void* chars;
	};
}
