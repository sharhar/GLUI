#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <GLUI/Renderer.h>
#include <GLUI/Utils.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>

using namespace glui;

int main() {
	GLUI::init();

	Window win("Test", 800, 600);

	std::string s = "hi";

	//input::keyboard::setTextCallback(&s);

	glEnable(GL_TEXTURE_2D);

	Font* font = new Font("arial.ttf");
	Color* color = new Color(1, 0, 1);

	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	while (win.isOpen()) {
		win.poll();
		glClear(GL_COLOR_BUFFER_BIT);

		double time = glfwGetTime();

		std::string text = "time since launch: " + std::to_string(time);

		color->r = cos(time)/2 + 0.5;
		color->g = sin(time)/2 + 0.5;
		color->b = 1;

		Renderer::render(text, 50, 50, 0.75, font, color);

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}