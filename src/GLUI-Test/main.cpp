#include <GLUI/GLUI.h>

//Only for testing, won't be needed when using GLUI
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>  
#include <math.h>		 
#include <iostream>		 

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
	BasicButtonDescriptor desc = { buttonText, 
		[]()->void {
			std::cout << "pressed!\n";
		} 
	};

	Button button(rect, layout, "Press", desc);

	TextBoxDescriptor tdesc = { textBoxText , color::lightGrey, color::darkGrey, color::darkGrey, 1, 2 };

	rect.h = 30;
	rect.x = 500;
	rect.w = 200;

	TextBox textBox(rect, layout, tdesc);

	rect.y = 200;

	TextBox textBox2(rect, layout, tdesc);

	GLPanel panel({ 10, 340, 250, 250 }, layout,
		[]()->void {
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.3f, 0.8f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(80, 1, 0.1, 1000);
			glMatrixMode(GL_MODELVIEW);
		},
		[]()->void { //Render function
			glPushMatrix();

			float rotSpeed = 90;

			glTranslatef(0, 0, -4);
			glRotatef(glfwGetTime() * rotSpeed / 1, 1, 0, 0);
			glRotatef(glfwGetTime() * rotSpeed / 2, 0, 1, 0);
			glRotatef(glfwGetTime() * rotSpeed / 3, 0, 0, 1);

			glCallList(box);

			glPopMatrix();
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