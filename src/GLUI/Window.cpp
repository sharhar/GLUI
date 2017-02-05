#include <GLUI/GLUI.h>
#include <GLUIExt.h>
#include <iostream>
#include <thread>

namespace glui {

	typedef struct WindowUserPointer {
		Window* window;
		int windowID;
	} GLFWuserPointer;

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

	static void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

		std::cout << "OpenGL Error:\n";
		std::cout << "=============\n";
		std::cout << " Object ID: " << id << "\n";
		std::cout << " Type: " << getGLTypeStr(type) << "\n";
		std::cout << " Source: " << getGLSourceStr(source) << "\n";
		std::cout << " Message: " << message << "\n\n";
	}

	void glfwDebugErrorCallback(int error, const char* description) {
		std::cout << "GLFW Error: " << error << "\n";
		std::cout << " Message: " << description << "\n";
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
		WindowUserPointer* win = (WindowUserPointer*)glfwGetWindowUserPointer(window);
		if (win->window != NULL) {
			win->window->m_focused = focused;
		}
		
		input::InputData::windowID = win->windowID;
	}

	void posCallback(GLFWwindow* window, int xpos, int ypos) {
		WindowUserPointer* win = (WindowUserPointer*)glfwGetWindowUserPointer(window);
		if (win->window != NULL) {
			win->window->m_xpos = xpos;
			win->window->m_ypos = ypos;
		}
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

		m_popups = std::vector<PopupHandle*>();

		if (resizeable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		else {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif
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

#ifdef _DEBUG
		glfwSetErrorCallback(glfwDebugErrorCallback);
#endif
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mousePosCallback);
		glfwSetCharCallback(window, textCallBack);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetWindowFocusCallback(window, focusCallback);
		glfwSetWindowPosCallback(window, posCallback);

		WindowUserPointer* userPointer = (WindowUserPointer*)malloc(sizeof(WindowUserPointer));
		userPointer->window = this;
		userPointer->windowID = 0;

		glfwSetWindowUserPointer(window, userPointer);

		m_window = window;

		if (icon != NULL) {
			glfwSetWindowIcon(window, iconNum, icon);
		}
	}

	void Window::poll() {
		glfwPollEvents();

		if (m_popups.size() > 0) {
			for (int i = 0; i < m_popups.size();i++) {
				PopupHandle* handle = m_popups[i];
				if (handle->closed && handle->window != NULL) {
					glfwDestroyWindow(handle->window);
					handle->window = NULL;
					handle->thread->join();
					delete handle->thread;
					handle->thread == NULL;
				}
			}
		}

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

	PopupHandle* Window::popup(PopupDescriptor desc, Theme* theme) {
		PopupHandle* handle = (PopupHandle*)malloc(sizeof(PopupHandle));

        handle->window = glfwCreateWindow(desc.width, desc.height, desc.title->c_str(), NULL, NULL);
		glfwHideWindow(handle->window);

		handle->closed = false;
		handle->result = -1;

		PopupDescriptor* pDesc = (PopupDescriptor*)malloc(sizeof(PopupDescriptor));
		pDesc->width = desc.width;
		pDesc->height = desc.height;
		pDesc->btnNum = desc.btnNum;
		pDesc->iconNum = desc.iconNum;

		pDesc->text = new std::string(*desc.text);
		pDesc->title = new std::string(*desc.title);
		pDesc->btnText = new std::string*[desc.btnNum];
		for (int i = 0; i < desc.btnNum;i++) {
			pDesc->btnText[i] = new std::string(*desc.btnText[i]);
		}

		pDesc->bodyTextStyle = (TextStyle*)malloc(sizeof(TextStyle));
		pDesc->bodyTextStyle->font = desc.bodyTextStyle->font;
		pDesc->bodyTextStyle->size = desc.bodyTextStyle->size;
		pDesc->bodyTextStyle->fontID = 1;

		pDesc->buttonTextStyle = (TextStyle*)malloc(sizeof(TextStyle));
		pDesc->buttonTextStyle->font = desc.buttonTextStyle->font;
		pDesc->buttonTextStyle->size = desc.buttonTextStyle->size;
		pDesc->buttonTextStyle->fontID = 1;

		pDesc->icon = (GLFWimage*)malloc(sizeof(GLFWimage)*desc.iconNum);
		memcpy(pDesc->icon, desc.icon, sizeof(GLFWimage)*desc.iconNum);
		
		handle->desc = pDesc;
		handle->theme = theme;

		Window* win = this;

		handle->thread = new std::thread([handle, win]() -> void {
			glfwMakeContextCurrent(handle->window);

			RendererGLData* renderData = Renderer::createRenderData(handle->desc->width, handle->desc->height);

			glfwSetWindowPos(handle->window,
				(win->getWidth() - handle->desc->width) / 2 + win->getX(),
				(win->getHeight() - handle->desc->height) / 2 + win->getY());

			glfwSetKeyCallback(handle->window, keyCallback);
			glfwSetMouseButtonCallback(handle->window, mouseButtonCallback);
			glfwSetCursorPosCallback(handle->window, mousePosCallback);
			glfwSetCharCallback(handle->window, textCallBack);
			glfwSetScrollCallback(handle->window, mouseScrollCallback);
			glfwSetWindowFocusCallback(handle->window, focusCallback);

			WindowUserPointer* userPointer = (WindowUserPointer*)malloc(sizeof(WindowUserPointer));
			userPointer->window = NULL;
			userPointer->windowID = 1;

			glfwSetWindowUserPointer(handle->window, userPointer);

			if (handle->desc->icon != NULL) {
				glfwSetWindowIcon(handle->window, handle->desc->iconNum, handle->desc->icon);
			}

			glfwShowWindow(handle->window);

			Renderer::setProjection(renderData, 0, handle->desc->width, 0, handle->desc->height, -1, 1);

			glViewport(0, 0, handle->desc->width, handle->desc->height);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Layout* layout = new AbsoluteLayout(NULL, handle->desc->width, handle->desc->height);

			std::vector<Button*> buttons;

			if (handle->desc->btnNum > 0) {
				int sizex = (handle->desc->width - 10) / handle->desc->btnNum - 10;

				for (int i = 0; i < handle->desc->btnNum; i++) {
					ButtonDescriptor bDesc = {
						*handle->desc->buttonTextStyle,
						[i, handle]() -> void {
						handle->result = i;
					}, 3, *handle->theme
					};

					Rectangle rect = { (float)(10 + (sizex + 10)*i), 10, (float)(sizex), 50 };

					Button* button = new Button(rect, layout, *handle->desc->btnText[i], bDesc, 1);

					buttons.push_back(button);
				}
			}

			handle->desc->bodyTextStyle->font->loadGL(1);
			handle->desc->buttonTextStyle->font->loadGL(1);

			glClearColor(handle->theme->popupBackground.r, handle->theme->popupBackground.g, handle->theme->popupBackground.b, 1);
			while (!glfwWindowShouldClose(handle->window) && handle->result == -1) {
				glClear(GL_COLOR_BUFFER_BIT);

				if (handle->desc->btnNum > 0) {
					for (int i = 0; i < handle->desc->btnNum; i++) {
						buttons[i]->poll();
					}
				}

				Renderer::beginDraw(renderData);

				Renderer::drawString(renderData, *handle->desc->text, 1, 10,
					handle->desc->height - handle->desc->bodyTextStyle->font->size - 10, handle->desc->bodyTextStyle->size,
					handle->desc->bodyTextStyle->font, handle->theme->popupText);

				if (handle->desc->btnNum > 0) {
					for (int i = 0; i < handle->desc->btnNum; i++) {
						buttons[i]->render();
					}
				}

				Renderer::endDraw();

				glfwSwapBuffers(handle->window);
			}

			handle->closed = true;
			glfwHideWindow(handle->window);

			handle->desc->bodyTextStyle->font->del(1);
			handle->desc->buttonTextStyle->font->del(1);

			Renderer::deleteRenderData(renderData);
		});

		m_popups.push_back(handle);

		return handle;
	}
}
