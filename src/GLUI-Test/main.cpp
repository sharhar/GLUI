#include <freetype-gl.h>
#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace glui;

int main() {
	GLUI::init();

	Window win("Test", 800, 600);
	
	FT_Library ft;
	FT_Init_FreeType(&ft);
	FT_Face face;
	if (FT_New_Face(ft, "arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	std::string s = "hi";

	input::keyboard::setTextCallback(&s);
	
	while (win.isOpen()) {
		win.poll();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}