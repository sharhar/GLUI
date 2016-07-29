#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <GLUI/Renderer.h>
#include <GLUI/Utils.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>
#include <GLUI/Button.h>
#include <GLUI/TextBox.h>

using namespace glui;

void func() {
	std::cout << "pressed!\n";
}

int main() {
	GLUI::init();

	Window win("Test", 800, 600);

	std::string s = "hi";

	Font* font = new Font("arial.ttf", 30);
	Color white = { 1,1,1 };
	Color black = { 0,0,0 };

	Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	TextStyle rtext = {
		30, font, white
	};

	TextStyle btext = {
		16, font, black
	};

	BasicButtonDescriptor desc = {
		rtext, layout, func 
	};

	Button button(rect, "Play", desc);

	TextBoxDescriptor tdesc = {
		btext, layout, 
		{ 0.75f, 0.75f, 0.75f },{ 0.2f, 0.2f, 0.2f },{ 0.2f, 0.2f, 0.2f },
		1, 2
	};

	rect.h = 30;
	rect.x = 500;
	rect.w = 200;

	TextBox textBox(rect, tdesc);

	Renderer::init();

	while (win.isOpen()) {
		win.poll();
		Renderer::clear({1, 1, 1});

		button.poll();
		textBox.poll();

		button.render();
		textBox.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}