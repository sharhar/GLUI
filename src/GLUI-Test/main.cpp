#include <Window.h>
#include <GLUI.h>
#include <iostream>

int main() {
	glui::GLUI::init();

	glui::Window win("Test", 800, 600);
	
	while (win.isOpen()) {
		win.poll();
	}

	win.destroy();

	glui::GLUI::destroy();

	return 0;
}