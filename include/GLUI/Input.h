#pragma once

#include <malloc.h>
#include <string>

namespace glui {
	namespace input {
		class Input {
		public:
			static void init();
			static Input* input;
			static std::string* textString;
			int* keys;
			int mousePosx;
			int mousePosy;
			bool mouseLeftDown;
		};

		namespace callbacks{
			void mousePosition(int posx, int posy);
			void mouseButton(int button, int state);
			void key(int key, int state);
			void text(unsigned int codepoint);
		}

		namespace keyboard {
			void setTextCallback(std::string* string);
		}

		namespace mouse {
			
		}
	}
}