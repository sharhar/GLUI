#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>

namespace glui {
	typedef void(*matrix_func)(void);
	typedef void(*render_func)(void);

	class GLPanel {
	private:
		unsigned int m_FBO;
		unsigned int m_tex;
		unsigned int m_dtex;
		matrix_func m_matrixFunc;
		render_func m_renderFunc;

		Rectangle m_rect;

		Layout* m_layout;

		void* m_glFuncs;
	public:
		GLPanel(Rectangle rect, Layout* layout, matrix_func matrixFunc, render_func renderFunc);
		void poll();
		void render();
	};
}