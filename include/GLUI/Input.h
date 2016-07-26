#pragma once

#include <malloc.h>

namespace glui {
	namespace input {
		class Input {
		public:
			static void init();
			static Input* input;
			int* keys;
			int mousePosx;
			int mousePosy;
			bool mouseLeftDown;
		};

		namespace callbacks{
			void mousePosition(int posx, int posy);
			void mouseButton(int button, int state);
			void key(int key, int state);
		}

		namespace keyboard {
			
		}

		namespace mouse {
			
		}
	}
}