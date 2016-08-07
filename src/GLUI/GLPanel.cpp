#include <GLUI/GLUI.h>
#include <GLUIExt.h>

typedef struct GLFuncs {
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
} GLFuncs;

namespace glui {

	GLPanel::GLPanel(Rectangle bounds, Layout* layout, glpanel_init_gl_func initGLFunc, glpanel_render_func renderFunc) :
		GLUIObject(bounds,layout){
		m_renderFunc = renderFunc;
		m_initGLFunc = initGLFunc;

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_bounds.w, m_bounds.h,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFuncs->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

		glGenTextures(1, &dtex);
		glBindTexture(GL_TEXTURE_2D, dtex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_bounds.w, m_bounds.h,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFuncs->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dtex, 0);

		glFuncs->glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_FBO = fbo;
		m_tex = tex;
		m_dtex = dtex;

		m_glInitList = glGenLists(1);
		glNewList(m_glInitList, GL_COMPILE);
		m_initGLFunc();
		glEndList();
	}

	void GLPanel::poll() {
		glBindTexture(GL_TEXTURE_2D, 0);
		((GLFuncs*)m_glFuncs)->glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_bounds.w, m_bounds.h);

		glCallList(m_glInitList);

		m_renderFunc();

		((GLFuncs*)m_glFuncs)->glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Renderer::reinit();
	}

	void GLPanel::render() {
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