#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>

namespace glui {
	struct GLFuncs;
	struct GLPanelMouseData;

	typedef void(*glpanel_render_func)(void);
	typedef void(*glpanel_init_gl_func)(void);
	typedef void(*glpanel_input_mouse_func)(GLPanelMouseData* data);

	typedef struct GLPanelMouseData {
		bool leftDown;
		Vector2f pos;
		Vector2f difference;
	} GLPanelMouseData;

	class GLPanel : public GLUIObject {
	private:
		unsigned int m_FBO;
		unsigned int m_tex;
		unsigned int m_dtex;
		unsigned int m_glInitList;
		glpanel_render_func m_renderFunc;
		glpanel_init_gl_func m_initGLFunc;
		glpanel_input_mouse_func m_inputMouseFunc;

		GLFuncs* m_glFuncs;

		GLPanelMouseData* m_mouseData;
		Vector2f m_fboSize;
	public:
		GLPanel(Rectangle bounds, Vector2f fboSize, Layout* layout, glpanel_init_gl_func initGLFunc, glpanel_render_func renderFunc, glpanel_input_mouse_func inputMouseFunc);
		void poll() override;
		void render() override;
		void renderDepth();
	};
}