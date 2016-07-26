#include <GLUI/GLUI.h>
#include <GLFW/glfw3.h>

namespace glui {
	void GLUI::init() {
		glfwInit();

		input::Input::init();
	}

	void GLUI::destroy() {
		glfwTerminate();
	}
}