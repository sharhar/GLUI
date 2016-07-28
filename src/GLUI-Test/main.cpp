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

	Font* font = new Font("arial.ttf", 32);
	Color* color = new Color(1, 1, 1);

	Renderer::init();

	while (win.isOpen()) {
		win.poll();
		Renderer::clear();

		double time = glfwGetTime();

		std::string text = "time: " + std::to_string(time);

		Renderer::drawString(text, 50, 100, 32, font, color);

		float top = 0.6f;
		float bottom = 0.4f;
		float outline = 0.45f;

		float x = 200;
		float y = 200;
		float w = 100;
		float h = 50;
		float ow = 3;

		glBegin(GL_QUADS);
		glColor3f(outline, outline, outline);
		glVertex2f(x - ow    , y - ow);
		glVertex2f(x - ow    , y + h + ow);
		glVertex2f(x + w + ow, y + h + ow);
		glVertex2f(x + w + ow, y - ow);

		glColor3f(bottom, bottom, bottom);
		glVertex2f(x, y);
		glColor3f(top, top, top);
		glVertex2f(x, y + h);
		glColor3f(top, top, top);
		glVertex2f(x + w, y + h);
		glColor3f(bottom, bottom, bottom);
		glVertex2f(x + w, y);
		glEnd();

		Renderer::drawString("Button", 210, 215, 28, font, color);

		top = 0.2f;
		bottom = 0.4f;
		x = 200;
		y = 400;

		glBegin(GL_QUADS);
		glColor3f(outline, outline, outline);
		glVertex2f(x - ow, y - ow);
		glVertex2f(x - ow, y + h + ow);
		glVertex2f(x + w + ow, y + h + ow);
		glVertex2f(x + w + ow, y - ow);

		glColor3f(bottom, bottom, bottom);
		glVertex2f(x, y);
		glColor3f(top, top, top);
		glVertex2f(x, y + h);
		glColor3f(top, top, top);
		glVertex2f(x + w, y + h);
		glColor3f(bottom, bottom, bottom);
		glVertex2f(x + w, y);
		glEnd();

		Renderer::drawString("Button", 210, 415, 28, font, color);

		glBindTexture(GL_TEXTURE_2D, 0);

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}