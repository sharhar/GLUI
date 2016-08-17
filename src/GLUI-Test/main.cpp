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

	Theme theme = {};
	theme.body     = color::lightGrey;
	theme.check    = color::black;
	theme.circle   = color::black;
	theme.hover    = color::grey;
	theme.outline  = color::black;
	theme.press    = color::darkGrey;
	theme.text     = color::black;

	TextStyle buttonText = { 30, font30 };
	TextStyle textBoxText = { 20, font20 };
	ButtonDescriptor desc = { buttonText, 
		[]()->void {
			std::cout << "pressed!\n";
		},
		3, theme
	};

	Button button(rect, layout, "Press", desc);

	TextBoxDescriptor tdesc = { textBoxText , 1, 2, theme };

	rect.h = 30;
	rect.x = 500;
	rect.w = 200;

	TextBox textBox(rect, layout, tdesc);

	rect.y = 200;

	TextBox textBox2(rect, layout, tdesc);

	GLPanel panel({ 10, 290, 300, 300 }, {300, 300}, layout,
		[]()->void { //Init gl function
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

			float rotSpeed = 30;

			glTranslatef(0, 0, -4);
			glRotatef(glfwGetTime() * rotSpeed / 1, 1, 0, 0);
			glRotatef(glfwGetTime() * rotSpeed / 2, 0, 1, 0);
			glRotatef(glfwGetTime() * rotSpeed / 3, 0, 0, 1);

			glCallList(box);

			glPopMatrix();
		},
		[](GLPanelMouseData* data)->void {
			std::cout << "(" << data->pos.x << ", " << data->pos.y << ")\n";
		});

	CheckBoxDescriptor checkBoxDesc = { 
		{ 30, font30 }, 
		3, 
		false,
		theme
	};

	CheckBox checkBox({100, 200, 30, 30}, layout, "toggle", checkBoxDesc);

	while (win.isOpen()) {
		win.poll();

		Renderer::clear(color::grey);

		button.poll();
		textBox.poll();
		textBox2.poll();
		panel.poll();
		checkBox.poll();

		button.render();
		textBox.render();
		textBox2.render();
		panel.render();
		checkBox.render();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}