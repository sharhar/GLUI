#pragma once

#include <GLUI/Utils.h>
#include <GLUI/Layout.h>
#include <GLUI/GLUIObject.h>
#include <functional>

namespace glui {
	struct GLFuncs;
	struct GLPanelMouseData;

	typedef struct GLPanelMouseData {
		bool leftDown;
		Vector2f pos;
		Vector2f difference;
		float scroll;
	} GLPanelMouseData;

	class GLPanel : public GLUIObject {
	private:
		unsigned int m_FBO;
		unsigned int m_tex;
        unsigned int m_dtex;
		std::function<void(void)> m_renderFunc;
		std::function<void(GLPanelMouseData* data)> m_inputMouseFunc;
        
		Theme m_theme;

		float m_prevScroll;
		GLPanelMouseData* m_mouseData;
		Vector2f m_fboSize;
	public:
		GLPanel(Rectangle bounds, Vector2f fboSize, Layout* layout, std::function<void(void)> renderFunc, std::function<void(GLPanelMouseData* data)> inputMouseFunc, Theme theme, int windowID);
		void pollFunction() override;
		void render() override;
		void renderDepth();
		unsigned int getFBO() { return m_FBO; }
	};
}
