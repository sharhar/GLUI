#pragma once

#include <string>
#include <functional>

namespace glui {
	namespace input {
		class InputData {
		public:
			static void init();
			static float scrollTotal;
			static std::string* textString;
			static size_t textInsertPoint;
			static unsigned char* keys;
			static int mousePosx;
			static int mousePosy;
			static bool mouseLeftDown;
			static std::function<void(void)>* textBoxEnterFunc;
		};

		namespace callbacks{
			void mousePosition(int posx, int posy);
			void mouseButton(int button, int state);
			void mouseScroll(float scroll);
			void key(int key, int state);
			void text(unsigned int codepoint);
		}

		namespace keyboard {
			void setTextCallback(std::string* string);
			void setTextInsertPoint(size_t index);
			void setEnterFunc(std::function<void(void)>* func);
		}

		namespace mouse {
			
		}
	}
}