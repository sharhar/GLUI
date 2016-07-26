#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <iostream>
#include <GLFW/glfw3.h>

using namespace glui;

int main() {
	GLUI::init();

	Window win("Test", 800, 600);

	std::string s = "hi";

	input::keyboard::setTextCallback(&s);
	
	while (win.isOpen()) {
		win.poll();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}