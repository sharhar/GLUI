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

	Window win("GLUI Test", 800, 600);

	Font* font = new Font("arial.ttf", 30);
	if (!font->inited()) {
		win.destroy();
#if defined(_WIN32) || defined (_WIN64)
		system("PAUSE");
#endif
		return -1;
	}

	Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	TextStyle buttonText = { 30, font, color::white };
	TextStyle textBoxText = { 16, font, color::black };
	BasicButtonDescriptor desc = { buttonText, layout, func };

	Button button(rect, "Play", desc);

	TextBoxDescriptor tdesc = { textBoxText, layout,  color::lightGrey, color::darkGrey, color::darkGrey, 1, 2 };

	rect.h = 30;
	rect.x = 500;
	rect.w = 200;

	TextBox textBox(rect, tdesc);

	rect.y = 300;

	TextBox textBox2(rect, tdesc);

	Renderer::init(&win);

	while (win.isOpen()) {
		win.poll();
		Renderer::clear({1, 1, 1});

		button.poll();
		button.render();
		
		textBox.poll();
		textBox.render();

		textBox2.poll();
		textBox2.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}