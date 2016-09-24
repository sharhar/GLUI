#pragma once

#include <string>
#include <vector>

namespace glui {
	typedef struct Color {
		float r, g, b;
	} Color;

	namespace color {
		static Color black         = { 0.00f, 0.00f, 0.00f };
		static Color white         = { 1.00f, 1.00f, 1.00f };
		static Color grey          = { 0.50f, 0.50f, 0.50f };
		static Color lightGrey     = { 0.75f, 0.75f, 0.75f };
		static Color darkGrey      = { 0.25f, 0.25f, 0.25f };
		static Color red           = { 1.00f, 0.00f, 0.00f };
		static Color green         = { 0.00f, 1.00f, 0.00f };
		static Color blue          = { 0.00f, 0.00f, 1.00f };
		static Color yellow        = { 1.00f, 1.00f, 0.00f };
		static Color cyan          = { 0.00f, 1.00f, 1.00f };
		static Color magenta       = { 1.00f, 0.00f, 1.00f };
	}

	class Font {
	private:
		bool m_inited;
		void* m_face;
	public:
		Font(std::string path, float a_size);

		std::vector<void*> chars;
		float size;
		int current;

		bool inited();
		void loadGL(int num);
		void del(int num);
	};

	typedef struct Rectangle {
		float x, y, w, h;
	} Rectangle;

	typedef struct Vector2f {
		float x, y;
	} Vector2f;

	typedef struct TextStyle {
		float size;
		Font* font;
	} TextStyle;

	typedef struct Theme {
		Color outline;
		Color body;
		Color hover;
		Color text;
		Color press;
		Color check;
		Color circle;
		Color popupBackground;
		Color popupText;
	} Theme;
}
