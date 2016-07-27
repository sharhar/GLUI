#pragma once

#include <string>

namespace glui {
	class Color {
	private:
		float m_r, m_g, m_b;
	public:
		Color(float r, float g, float b);
	};

	class Font {
	public:
		Font(std::string path);
	};
}
