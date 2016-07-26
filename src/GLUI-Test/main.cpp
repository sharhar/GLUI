#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <iostream>
#include <GLFW/glfw3.h>

int* lastKeys = new int[65536];
int key = 0;

bool check() {
	bool pressed = false;
	for (int i = 0; i < 65536;i++) {
		if (glui::input::Input::input->keys[i] && !lastKeys[i]) {
			pressed = true;
			key = i;
			break;
		}
	}
	memcpy(lastKeys, glui::input::Input::input->keys, sizeof(int) * 65536);
	return pressed;
}

void func() {
	std::cout << "pressed" << key << "\n";
}

int main() {
	glui::GLUI::init();

	glui::Window win("Test", 800, 600);
	
	glui::Listener listener(check, func);
	
	while (win.isOpen()) {
		win.poll();
		listener.tick();
	}

	win.destroy();

	glui::GLUI::destroy();

	return 0;
}