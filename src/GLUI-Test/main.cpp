#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <iostream>
#include <iostream>
#include <GLFW/glfw3.h>

int main() {
	glui::GLUI::init();

	glui::Window win("Test", 800, 600);
	
	std::string s = "hi";

	glui::input::keyboard::setTextCallback(&s);
	
	while (win.isOpen()) {
		win.poll();
		std::cout << s << "\n";
	}

	win.destroy();

	glui::GLUI::destroy();

	return 0;
}