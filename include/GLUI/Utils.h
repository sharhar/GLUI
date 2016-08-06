#pragma once

#include <string>

namespace glui {
	typedef struct Color {
		float r, g, b;
	} Color;

	namespace color {
		static Color black = { 0, 0, 0 };
		static Color white = { 1, 1, 1 };
		static Color lightGrey = { 0.75f, 0.75f, 0.75f };
		static Color darkGrey = { 0.25f, 0.25f, 0.25f };
	}

	class Font {
	private:
		bool m_inited;
	public:
		Font(std::string path, float a_size);

		void* chars;
		float size;

		bool inited();
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
