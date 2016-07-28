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

using namespace glui;

void func() {
	std::cout << "pressed!\n";
}

int main() {
	GLUI::init();

	Window win("Test", 800, 600);

	std::string s = "hi";

	Font* font = new Font("arial.ttf", 32);
	Color white = {1,1,1};

	Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	TextStyle rtext = {
		30, font, white
	};

	BasicButtonDescriptor desc = {
		rtext, layout, func 
	};

	Button button(rect, "Play", desc);

	rect.x = 500;

	Button button2(rect, "Play", desc);

	Renderer::init();

	while (win.isOpen()) {
		win.poll();
		Renderer::clear();

		button.poll();
		button.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}