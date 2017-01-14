#include <GLUI/GLUI.h>
#include <GLUIExt.h>
#include <iostream>
#include <thread>

namespace glui {
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

	Window::Window(const char* title, int width, int height, bool resizeable, int iconNum, GLFWimage* icon) {
		m_width = width;
		m_height = height;

		if (resizeable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		} else {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);

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

		GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			//std::cout << "GLError: " << err << "\n";
		}
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

	void* Window::getGLFWwindow() {
		return m_window;
	}

	int Window::popup(PopupDescriptor desc, Theme theme) {
		int result = -1;

		input::InputData::mouseLeftDown = false;

		std::thread thread([&]() -> void {
			GLFWwindow* window = glfwCreateWindow(desc.width, desc.height, desc.title, NULL, NULL);
			glfwMakeContextCurrent(window);

			glfwSetWindowPos(window,
				(desc.window->getWidth() - desc.width) / 2 + desc.window->getX(),
				(desc.window->getHeight() - desc.height) / 2 + desc.window->getY());

			glfwSetKeyCallback(window, keyCallback);
			glfwSetMouseButtonCallback(window, mouseButtonCallback);
			glfwSetCursorPosCallback(window, mousePosCallback);
			glfwSetCharCallback(window, textCallBack);
			glfwSetScrollCallback(window, mouseScrollCallback);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, desc.width, 0, desc.height, -1, 1);
			glMatrixMode(GL_MODELVIEW);

			glViewport(0, 0, desc.width, desc.height);

			glEnable(GL_TEXTURE_2D);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Layout* layout = new AbsoluteLayout(NULL, desc.width, desc.height);

			desc.bodyTextStyle.font->loadGL(1);
			desc.bodyTextStyle.font->current = 1;

			desc.buttonTextStyle.font->loadGL(1);
			desc.buttonTextStyle.font->current = 1;

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

				Renderer::drawString(desc.text, 10, desc.height - desc.bodyTextStyle.font->size - 10, desc.bodyTextStyle.size , desc.bodyTextStyle.font, &theme.popupText);

				if (desc.btnNum > 0) {
					for (int i = 0; i < desc.btnNum; i++) {
						buttons[i]->poll();
					}

					for (int i = 0; i < desc.btnNum; i++) {
						buttons[i]->render();
					}
				}

				glfwSwapBuffers(window);
			}

			if (result == -1) {
				result = 0;
			}

			desc.buttonTextStyle.font->current = 0;
			desc.bodyTextStyle.font->current = 0;
			glfwDestroyWindow(window);

			desc.buttonTextStyle.font->del(1);
			desc.buttonTextStyle.font->chars.pop_back();

			if (desc.bodyTextStyle.font->chars.size() >= 2) {
				desc.bodyTextStyle.font->del(1);
				desc.bodyTextStyle.font->chars.pop_back();
			}
		});

		thread.join();

		input::InputData::mouseLeftDown = false;

		return result;
	}
}