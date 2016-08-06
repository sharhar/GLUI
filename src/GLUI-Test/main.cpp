#include <GLUI/Window.h>
#include <GLUI/GLUI.h>
#include <GLUI/Input.h>
#include <GLUI/Listener.h>
#include <GLUI/Renderer.h>
#include <GLUI/Utils.h>
#include <iostream>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <math.h>
#include <GLUI/Button.h>
#include <GLUI/TextBox.h>
#include <GLUI/GLPanel.h>

using namespace glui;

void renderFunc() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.8f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	float rotSpeed = 90;

	glTranslatef(0, 0, -4);
	glRotatef(glfwGetTime() * rotSpeed, 1, 0, 0);
	glRotatef(glfwGetTime() * rotSpeed/2, 0, 1, 0);
	glRotatef(glfwGetTime() * rotSpeed/3, 0, 0, 1);

	glBegin(GL_QUADS);

	glColor3f(1, 0, 0);
	glVertex3f(-1, -1,  1);
	glVertex3f( 1, -1,  1);
	glVertex3f( 1,  1,  1);
	glVertex3f(-1,  1,  1);

	glColor3f(1, 1, 0);
	glVertex3f(-1, -1,  1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1,  1, -1);
	glVertex3f(-1,  1,  1);

	glColor3f(0, 1, 0);
	glVertex3f(-1, -1, -1);
	glVertex3f( 1, -1, -1);
	glVertex3f( 1,  1, -1);
	glVertex3f(-1,  1, -1);

	glColor3f(1, 0, 1);
	glVertex3f( 1, -1,  1);
	glVertex3f( 1, -1, -1);
	glVertex3f( 1,  1, -1);
	glVertex3f( 1,  1,  1);

	glColor3f(0, 1, 1);
	glVertex3f(-1,  1,  1);
	glVertex3f(-1,  1, -1);
	glVertex3f( 1,  1, -1);
	glVertex3f( 1,  1,  1);

	glColor3f(0, 0, 1);
	glVertex3f(-1, -1,  1);
	glVertex3f(-1, -1, -1);
	glVertex3f( 1, -1, -1);
	glVertex3f( 1, -1,  1);

	glEnd();

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
}

int main() {
	GLUI::init();

	Window win("GLUI Test", 800, 600);

	Renderer::init(&win);

	Font* font30 = new Font("arial.ttf", 30);
	
	if (!font30->inited()) {
		win.destroy();
#if defined(_WIN32) || defined (_WIN64)
		system("PAUSE");
#endif
		return -1;
	}

	Font* font20 = new Font("arial.ttf", 20);

	if (!font20->inited()) {
		win.destroy();
#if defined(_WIN32) || defined (_WIN64)
		system("PAUSE");
#endif
		return -1;
	}

	Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	TextStyle buttonText = { 30, font30, color::white };
	TextStyle textBoxText = { 20, font20, color::black };
	BasicButtonDescriptor desc = { buttonText, layout, 
		[]()->void {
			std::cout << "pressed!\n";
		} 
	};

	Button button(rect, "Press", desc);

	TextBoxDescriptor tdesc = { textBoxText, layout,  color::lightGrey, color::darkGrey, color::darkGrey, 1, 2 };

	rect.h = 30;
	rect.x = 500;
	rect.w = 200;

	TextBox textBox(rect, tdesc);

	rect.y = 300;

	TextBox textBox2(rect, tdesc);

	GLPanel panel({ 10, 200, 390, 390 }, layout, 
		[]() -> void{
			glLoadIdentity();
			gluPerspective(80, 1, 0.1, 1000);
		}
	, renderFunc);

	while (win.isOpen()) {
		win.poll();

		Renderer::clear({1, 1, 1});

		button.poll();
		button.render();
		
		textBox.poll();
		textBox.render();

		textBox2.poll();
		textBox2.render();

		panel.poll();
		panel.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}