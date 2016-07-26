#include <GLUI/Input.h>
#include <GLFW/glfw3.h>

namespace glui {
	namespace input {
		Input* Input::input = 0;

		void Input::init() {
			Input::input = new Input();
			Input::input->mousePosx = -1;
			Input::input->mousePosy = -1;
			Input::input->mouseLeftDown = false;
			Input::input->keys = new int[65536];
		}

		void callbacks::mousePosition(int posx, int posy) {
			Input::input->mousePosx = posx;
			Input::input->mousePosy = posy;
		}

		void callbacks::mouseButton(int button, int state) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (state == GLFW_PRESS) {
					Input::input->mouseLeftDown = true;
				}
				else if (state == GLFW_RELEASE) {
					Input::input->mouseLeftDown = false;
				}
			}
			
		}

		void callbacks::key(int key, int state) {
			Input::input->keys[key] = state;
		}
	}
}