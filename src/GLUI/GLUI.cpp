#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	void* GLUI::data = 0;

	void GLUI::init() {
		glfwInit();

		input::InputData::init();

		GLUIData* gData = (GLUIData*) malloc(sizeof(GLUIData));

		if (FT_Init_FreeType(&(gData->ft))) {
			fprintf(stderr, "Could not init freetype library\n");
		}

		data = (void*) gData;
	}

	void GLUI::destroy() {
		GLUIData* gData = ((GLUIData*)data);
		FT_Done_FreeType(gData->ft);
		glfwTerminate();
	}
}