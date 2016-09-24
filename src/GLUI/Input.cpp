#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	namespace input {
		std::string* InputData::textString = 0;
		size_t InputData::textInsertPoint = 0;
		int InputData::mousePosx = -1;
		int InputData::mousePosy = -1;
		float InputData::scrollTotal = 0;
		bool InputData::mouseLeftDown = false;
		unsigned char* InputData::keys = new unsigned char[GLFW_KEY_LAST];
		std::function<void(void)>* InputData::textBoxEnterFunc = NULL;

		void InputData::init() {
			memset(InputData::keys, 0, sizeof(unsigned char) * GLFW_KEY_LAST);
		}

		void callbacks::mousePosition(int posx, int posy) {
			InputData::mousePosx = posx;
			InputData::mousePosy = posy;
		}

		void callbacks::mouseButton(int button, int state) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (state == GLFW_PRESS) {
					InputData::mouseLeftDown = true;
				}
				else if (state == GLFW_RELEASE) {
					InputData::mouseLeftDown = false;
				}
			}
			
		}

		void callbacks::mouseScroll(float scroll) {
			InputData::scrollTotal += scroll;
		}

		void callbacks::key(int key, int state) {
			InputData::keys[key] = state;
			if (InputData::textString != NULL && state == GLFW_PRESS) {
				if (key == GLFW_KEY_BACKSPACE && InputData::textInsertPoint != 0) {
					InputData::textString->replace(InputData::textInsertPoint - 1, 1, "");
				} else if (key == GLFW_KEY_ENTER && InputData::textBoxEnterFunc != NULL) {
					(*InputData::textBoxEnterFunc)();
				}
			}
		}

		void callbacks::text(unsigned int codepoint) {
			if (InputData::textString != 0) {
				if (InputData::textInsertPoint > InputData::textString->size()) {
					InputData::textInsertPoint = InputData::textString->size();
				}

				InputData::textString->insert(InputData::textInsertPoint, std::string(1, (char) codepoint));
			}
		}

		void keyboard::setTextCallback(std::string* string) {
			InputData::textString = string;
		}

		void keyboard::setTextInsertPoint(size_t index) {
			InputData::textInsertPoint = index;
		}

		void keyboard::setEnterFunc(std::function<void(void)>* func) {
			InputData::textBoxEnterFunc = func;
		}
	}
}