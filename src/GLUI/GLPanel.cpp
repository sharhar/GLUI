#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {

	typedef struct GLFuncs {
		PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
		PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
	} GLFuncs;

#define glGenFramebuffers m_glFuncs->glGenFramebuffers
#define glBindFramebuffer m_glFuncs->glBindFramebuffer
#define glFramebufferTexture m_glFuncs->glFramebufferTexture

	GLPanel::GLPanel(Rectangle bounds, Vector2f fboSize, Layout* layout, std::function<void(void)> initGLFunc, std::function<void(void)> renderFunc, std::function<void(GLPanelMouseData* data)> inputMouseFunc, Theme theme) :
		GLUIObject(bounds,layout){
		m_renderFunc = renderFunc;
		m_initGLFunc = initGLFunc;
		m_inputMouseFunc = inputMouseFunc;
		m_fboSize = fboSize;
		m_theme = theme;
		m_prevScroll = 0;

		m_mouseData = (GLPanelMouseData*)malloc(sizeof(GLPanelMouseData));
		m_mouseData->difference = {0, 0};
		m_mouseData->pos = {-1, -1};
		m_mouseData->leftDown = false;
		m_mouseData->scroll = 0;

		m_glFuncs = (GLFuncs*)malloc(sizeof(GLFuncs));

		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glfwGetProcAddress("glGenFramebuffers");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glfwGetProcAddress("glBindFramebuffer");
		glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glfwGetProcAddress("glFramebufferTexture");

		GLuint fbo;
		GLuint tex;
		GLuint dtex;

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_fboSize.x, m_fboSize.y,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

		glGenTextures(1, &dtex);
		glBindTexture(GL_TEXTURE_2D, dtex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_fboSize.x, m_fboSize.y,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dtex, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_FBO = fbo;
		m_tex = tex;
		m_dtex = dtex;

		m_glInitList = glGenLists(1);
		glNewList(m_glInitList, GL_COMPILE);
		m_initGLFunc();
		glEndList();
	}

	void GLPanel::poll() {
		if (m_inputMouseFunc != NULL) {
			float posx = input::InputData::mousePosx * m_layout->getScaleX();
			float posy = input::InputData::mousePosy * m_layout->getScaleY();

			bool down = input::InputData::mouseLeftDown;

			bool hovering = posx >= m_bounds.x &&
				posx <= m_bounds.x + m_bounds.w &&
				m_layout->getHeight() - posy >= m_bounds.y &&
				m_layout->getHeight() - posy <= m_bounds.y + m_bounds.h;

			posx -= m_bounds.x;
			posy -= (m_layout->getHeight() - (m_bounds.y + m_bounds.h));

			if (hovering) {
				if (m_mouseData->pos.x != posx || m_mouseData->pos.y != posy 
					|| m_mouseData->leftDown != down || m_mouseData->scroll != input::InputData::scrollTotal) {
					if (m_mouseData->difference.x != -1) {
						m_mouseData->difference = { posx - m_mouseData->pos.x, posy - m_mouseData->pos.y };
					}
					else {
						m_mouseData->difference = { 0, 0 };
					}
					m_mouseData->pos = { posx, posy };
					m_mouseData->leftDown = down;
					m_mouseData->scroll = input::InputData::scrollTotal - m_prevScroll;

					m_inputMouseFunc(m_mouseData);
				}

				m_prevScroll = input::InputData::scrollTotal;
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_fboSize.x, m_fboSize.y);

		glCallList(m_glInitList);

		m_renderFunc();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Renderer::reinit();
	}

	void GLPanel::render() {
		glColor3f(m_theme.outline.r, m_theme.outline.g, m_theme.outline.b);

		glBegin(GL_QUADS);
		glVertex2f(m_bounds.x - 3, m_bounds.y + m_bounds.h + 3);
		glVertex2f(m_bounds.x + m_bounds.w + 3, m_bounds.y + m_bounds.h + 3);
		glVertex2f(m_bounds.x + m_bounds.w + 3, m_bounds.y - 3);
		glVertex2f(m_bounds.x - 3, m_bounds.y - 3);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, m_tex);

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex2f(m_bounds.x             , m_bounds.y + m_bounds.h);
		glTexCoord2d(1.0, 1.0); glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y + m_bounds.h);
		glTexCoord2d(1.0, 0.0); glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y             );
		glTexCoord2d(0.0, 0.0); glVertex2f(m_bounds.x             , m_bounds.y             );
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLPanel::renderDepth() {
		glBindTexture(GL_TEXTURE_2D, m_dtex);

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex2f(m_bounds.x             , m_bounds.y + m_bounds.h);
		glTexCoord2d(1.0, 1.0); glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y + m_bounds.h);
		glTexCoord2d(1.0, 0.0); glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y             );
		glTexCoord2d(0.0, 0.0); glVertex2f(m_bounds.x             , m_bounds.y             );
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}