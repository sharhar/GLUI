#include <GLUI/GLUI.h>
#include <GLUIExt.h>
#include <iostream>
#include <thread>

namespace glui {
    
#ifdef _DEBUG
	const char*
		getGLSourceStr(GLenum source)
	{
		static const char* sources[] = {
			"API",   "Window System", "Shader Compiler", "Third Party", "Application",
			"Other", "Unknown"
		};

		int str_idx =
			min(source - GL_DEBUG_SOURCE_API,
				sizeof(sources) / sizeof(const char *));

		return sources[str_idx];
	}

	const char*
		getGLTypeStr(GLenum type)
	{
		static const char* types[] = {
			"Error",       "Deprecated Behavior", "Undefined Behavior", "Portability",
			"Performance", "Other",               "Unknown"
		};

		int str_idx =
			min(type - GL_DEBUG_TYPE_ERROR,
				sizeof(types) / sizeof(const char *));

		return types[str_idx];
	}

	const char*
		getGLSeverityStr(GLenum severity)
	{
		static const char* severities[] = {
			"High", "Medium", "Low", "Unknown"
		};

		int str_idx =
			min(severity - GL_DEBUG_SEVERITY_HIGH,
				sizeof(severities) / sizeof(const char *));

		return severities[str_idx];
	}

	static void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		
		std::cout << "OpenGL Error:\n";
		std::cout << "=============\n";
		std::cout << " Object ID: " << id << "\n";
		//std::cout << " Severity: " << getGLSeverityStr(severity) << "\n";
		std::cout << " Type: " << getGLTypeStr(type) << "\n";
		std::cout << " Source: " << getGLSourceStr(source) << "\n";
		std::cout << " Message: " << message << "\n\n";
	}
#endif

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::callbacks::key(key, action);
	}

	static void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
		input::callbacks::mousePosition((int)xpos, (int)ypos);
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		input::callbacks::mouseButton(button, action);
	}

	static void textCallBack(GLFWwindow* window, unsigned int codepoint) {
		input::callbacks::text(codepoint);
	}

	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		input::callbacks::mouseScroll(yoffset);
	}

	void focusCallback(GLFWwindow* window, int focused) {
		Window* win = (Window*) glfwGetWindowUserPointer(window);
		win->m_focused = focused;
	}

	void posCallback(GLFWwindow* window, int xpos, int ypos) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_xpos = xpos;
		win->m_ypos = ypos;
	}

	WindowCallbacks Window::getCallbacks() {
		WindowCallbacks result = {};
		result.key = input::callbacks::key;
		result.mousePos = input::callbacks::mousePosition;
		result.mouseButton = input::callbacks::mouseButton;
		result.mouseScroll = input::callbacks::mouseScroll;
		result.text = input::callbacks::text;
		return result;
	}

	Window::Window(GLFWwindow* glfwWin) {
		m_window = glfwWin;
		glfwGetWindowSize(glfwWin, &m_width, &m_height);
	}

	Window::Window(const char* title, int width, int height, bool resizeable, int iconNum, GLFWimage* icon) {
		m_width = width;
		m_height = height;

		if (resizeable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		} else {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Could not load OpenGL!\n";
			exit(-1);
		}
#ifdef _DEBUG
		PFNGLDEBUGMESSAGECALLBACKPROC pfnDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)
			glfwGetProcAddress("glDebugMessageCallback");

		if (pfnDebugMessageCallback) {
			pfnDebugMessageCallback(glDebugCallback, NULL);
		}
#endif
		glfwSwapInterval(1);

		const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_xpos = (vidMode->width - width) / 2;
		m_ypos = (vidMode->height - height) / 2;

		glfwSetWindowPos(window, m_xpos, m_ypos);
		
		m_focused = 1;

		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mousePosCallback);
		glfwSetCharCallback(window, textCallBack);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetWindowFocusCallback(window, focusCallback);
		glfwSetWindowPosCallback(window, posCallback);

		glfwSetWindowUserPointer(window, this);

		m_window = window;

		if (icon != NULL) {
			glfwSetWindowIcon(window, iconNum, icon);
		}
	}

	void Window::poll() {
		glfwPollEvents();

#ifdef _DEBUG
		GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			
		}
#endif
	}

	void Window::swap() {
		glfwSwapBuffers(m_window);
	}

	bool Window::isOpen() {
		return !glfwWindowShouldClose(m_window);
	}

	void Window::destroy() {
		glfwDestroyWindow(m_window);
	}

	GLFWwindow* Window::getGLFWwindow() {
		return m_window;
	}

	int Window::popup(PopupDescriptor desc, Theme theme) {
		int result = -1;

		input::InputData::mouseLeftDown = false;
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        GLFWwindow* window = glfwCreateWindow(desc.width, desc.height, desc.title, NULL, desc.window->getGLFWwindow());
        glfwMakeContextCurrent(window);

        glfwSetWindowPos(window,
            (desc.window->getWidth() - desc.width) / 2 + desc.window->getX(),
            (desc.window->getHeight() - desc.height) / 2 + desc.window->getY());

        glfwSetKeyCallback(window, keyCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetCursorPosCallback(window, mousePosCallback);
        glfwSetCharCallback(window, textCallBack);
        glfwSetScrollCallback(window, mouseScrollCallback);

        Renderer::setProjection(0, desc.width, 0, desc.height, -1, 1);
        
        glViewport(0, 0, desc.width, desc.height);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Layout* layout = new AbsoluteLayout(NULL, desc.width, desc.height);

        std::vector<Button*> buttons;

        if (desc.btnNum > 0) {
            int sizex = (desc.width - 10) / desc.btnNum - 10;

            for (int i = 0; i < desc.btnNum;i++) {
                ButtonDescriptor bDesc = {
                    desc.buttonTextStyle,
                    [i, &result]() -> void {
                        result = i;
                    }, 3, theme
                };

                Rectangle rect = { (float) (10 + (sizex + 10)*i), 10, (float)(sizex), 50 };

                Button* button = new Button(rect, layout, std::string(desc.btnText[i]), bDesc);
                
                buttons.push_back(button);
            }
        }
        
            
        glClearColor(theme.popupBackground.r, theme.popupBackground.g, theme.popupBackground.b, 1);
        while (!glfwWindowShouldClose(window) && result == -1) {
            glfwPollEvents();
                
            glClear(GL_COLOR_BUFFER_BIT);
            
            if (desc.btnNum > 0) {
                for (int i = 0; i < desc.btnNum; i++) {
                    buttons[i]->poll();
                }
            }
            
            Renderer::beginDraw();
            
            Renderer::drawString(desc.text, 10, desc.height - desc.bodyTextStyle.font->size - 10, desc.bodyTextStyle.size , desc.bodyTextStyle.font, theme.popupText);

            if (desc.btnNum > 0) {
                for (int i = 0; i < desc.btnNum; i++) {
                        buttons[i]->render();
                }
                
            }
            
            Renderer::endDraw();

            glfwSwapBuffers(window);
        }

        if (result == -1) {
            result = 0;
        }
        
        glfwDestroyWindow(window);
        
        glfwMakeContextCurrent(desc.window->getGLFWwindow());
        
        Renderer::setProjection(0, desc.window->getWidth(), 0, desc.window->getHeight(), -1, 1);
        
		input::InputData::mouseLeftDown = false;

		return result;
	}
}
