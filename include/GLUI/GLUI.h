#pragma once

#include <GLUI/Button.h>
#include <GLUI/GLPanel.h>
#include <GLUI/Input.h>
#include <GLUI/Layout.h>
#include <GLUI/Listener.h>
#include <GLUI/Renderer.h>
#include <GLUI/TextBox.h>
#include <GLUI/Utils.h>
#include <GLUI/Window.h>

namespace glui {
	class GLUI {
	public:
		static void init();
		static void destroy();

		static void* data;
	};
}