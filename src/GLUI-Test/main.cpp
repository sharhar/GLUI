#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <GLUI/Renderer.h>
#include <GLUI/Utils.h>
#include <iostream>
#include <GLFW/glfw3.h>

using namespace glui;

int main() {
	GLUI::init();

	Window win("Test", 800, 600);

	std::string s = "hi";

	//input::keyboard::setTextCallback(&s);

	glEnable(GL_TEXTURE_2D);

	Font* font = new Font("arial.ttf");

	glClearColor(1, 0, 1, 1);
	
	while (win.isOpen()) {
		win.poll();
		glClear(GL_COLOR_BUFFER_BIT);

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}