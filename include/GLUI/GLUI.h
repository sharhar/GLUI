#pragma once

#include <GLUI/Input.h>

namespace glui {
	class GLUI {
	public:
		static void init();
		static void destroy();

		static void* data;
	};
}