#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	namespace input {
		std::string* Input::textString = 0;
		size_t Input::textInsertPoint = 0;
		int Input::mousePosx = -1;
		int Input::mousePosy = -1;
		bool Input::mouseLeftDown = false;
		unsigned char* Input::keys = new unsigned char[GLFW_KEY_LAST];

		void Input::init() {
			memset(Input::keys, 0, sizeof(unsigned char) * GLFW_KEY_LAST);
		}

		void callbacks::mousePosition(int posx, int posy) {
			Input::mousePosx = posx;
			Input::mousePosy = posy;
		}

		void callbacks::mouseButton(int button, int state) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (state == GLFW_PRESS) {
					Input::mouseLeftDown = true;
				}
				else if (state == GLFW_RELEASE) {
					Input::mouseLeftDown = false;
				}
			}
			
		}

		void callbacks::key(int key, int state) {
			Input::keys[key] = state;
			if (Input::textString != 0 && key == GLFW_KEY_BACKSPACE && state == GLFW_PRESS && Input::textString->size() != 0) {
				Input::textString->replace(Input::textInsertPoint-1, 1, "");
			}
		}

		void callbacks::text(unsigned int codepoint) {
			if (Input::textString != 0) {
				char* insertion = new char[1];
				insertion[0] = (char) codepoint;
				Input::textString->insert(Input::textInsertPoint, std::string(insertion));
				delete[] insertion;
			}
		}

		void keyboard::setTextCallback(std::string* string) {
			Input::textString = string;
		}

		void keyboard::setTextInsertPoint(size_t index) {
			Input::textInsertPoint = index;
		}
	}
}