#include <GLUI/GLUI.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h> //Only for testing, won't be needed when using GLUI
#include <math.h>		//Only for testing, won't be needed when using GLUI
#include <iostream>		//Only for testing, won't be needed when using GLUI

using namespace glui;

GLuint box;

void cube() {
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
}

int main() {
	GLUI::init();

	Window win("GLUI Test", 800, 600);

	Renderer::init(&win);

	box = glGenLists(1);
	glNewList(box, GL_COMPILE);
	cube();
	glEndList();

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
		[]()->void { //Render function
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.3f, 0.8f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(80, 1, 0.1, 1000);
			glMatrixMode(GL_MODELVIEW);

			glPushMatrix();

			float rotSpeed = 90;

			glTranslatef(0, 0, -4);
			glRotatef(glfwGetTime() * rotSpeed, 1, 0, 0);
			glRotatef(glfwGetTime() * rotSpeed / 2, 0, 1, 0);
			glRotatef(glfwGetTime() * rotSpeed / 3, 0, 0, 1);

			glCallList(box);

			glPopMatrix();

			glDisable(GL_DEPTH_TEST);
		});

	while (win.isOpen()) {
		win.poll();

		Renderer::clear(color::grey);

		button.poll();
		textBox.poll();
		textBox2.poll();
		panel.poll();

		button.render();
		textBox.render();
		textBox2.render();
		panel.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}