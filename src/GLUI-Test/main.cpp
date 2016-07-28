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

	glEnable(GL_TEXTURE_2D);

	Font* font = new Font("arial.ttf", 32);
	Color* color = new Color(1, 1, 1);

	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	while (win.isOpen()) {
		win.poll();
		glClear(GL_COLOR_BUFFER_BIT);

		double time = glfwGetTime();

		std::string text = "time: " + std::to_string(time);

		Renderer::render(text, 50, 100, 32, font, color);

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}