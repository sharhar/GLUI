#include <GLUI/GLUI.h>
#include <GLUIExt.h>

typedef struct GLFuncs {
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
} GLFuncs;

namespace glui {

	GLPanel::GLPanel(Rectangle rect, Layout* layout, glpanel_render_func renderFunc) {
		m_rect = rect;
		m_layout = layout;
		m_renderFunc = renderFunc;

		GLFuncs* glFuncs = (GLFuncs*)malloc(sizeof(GLFuncs));

		glFuncs->glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glfwGetProcAddress("glGenFramebuffers");
		glFuncs->glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glfwGetProcAddress("glBindFramebuffer");
		glFuncs->glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glfwGetProcAddress("glFramebufferTexture");
		glFuncs->glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glfwGetProcAddress("glCheckFramebufferStatus");

		m_glFuncs = (void*)glFuncs;

		GLuint fbo;
		GLuint tex;
		GLuint dtex;

		glFuncs->glGenFramebuffers(1, &fbo);
		glFuncs->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_rect.w, m_rect.h,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFuncs->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

		glGenTextures(1, &dtex);
		glBindTexture(GL_TEXTURE_2D, dtex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_rect.w, m_rect.h,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFuncs->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dtex, 0);

		glFuncs->glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_FBO = fbo;
		m_tex = tex;
		m_dtex = dtex;
	}

	void GLPanel::poll() {
		glBindTexture(GL_TEXTURE_2D, 0);
		((GLFuncs*)m_glFuncs)->glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_rect.w, m_rect.h);

		m_renderFunc();

		((GLFuncs*)m_glFuncs)->glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_layout->getWidth(), m_layout->getHeight());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_layout->getWidth(), 0, m_layout->getHeight(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	void GLPanel::render() {
		glBindTexture(GL_TEXTURE_2D, m_tex);

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex2f(m_rect.x           , m_rect.y + m_rect.h);
		glTexCoord2d(1.0, 1.0); glVertex2f(m_rect.x + m_rect.w, m_rect.y + m_rect.h);
		glTexCoord2d(1.0, 0.0); glVertex2f(m_rect.x + m_rect.w, m_rect.y           );
		glTexCoord2d(0.0, 0.0); glVertex2f(m_rect.x           , m_rect.y           );
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}