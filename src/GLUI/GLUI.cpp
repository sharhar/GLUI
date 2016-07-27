#include <GLUI/GLUI.h>
#include <GLUI/DONOTINCLUDE.h>

namespace glui {
	void* GLUI::data = 0;

	void GLUI::init() {
		glewInit();
		glfwInit();

		input::Input::init();

		GLUIData* gData = (GLUIData*) malloc(sizeof(GLUIData));

		if (FT_Init_FreeType(&(gData->ft))) {
			fprintf(stderr, "Could not init freetype library\n");
		}

		data = (void*) gData;
	}

	void GLUI::destroy() {
		glfwTerminate();
		FT_Done_FreeType(((GLUIData*) data)->ft);
	}
}