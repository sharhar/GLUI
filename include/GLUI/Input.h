#pragma once

#include <string>

namespace glui {
	namespace input {
		class Input {
		public:
			static void init();
			static float scrollTotal;
			static std::string* textString;
			static size_t textInsertPoint;
			static unsigned char* keys;
			static int mousePosx;
			static int mousePosy;
			static bool mouseLeftDown;
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
		}

		namespace mouse {
			
		}
	}
}