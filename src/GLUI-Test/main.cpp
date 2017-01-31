#include <GLUI/GLUI.h>

#include <math.h>		   //----------------------------------------------------------
#include <GLFW/glfw3.h>    //    Only for testing, won't be needed when using GLUI
#include <iostream>		   //----------------------------------------------------------

using namespace glui;

int main() {
	GLUI::init();

	Window win("GLUI Test", 800, 600, false, 0, NULL);

	Renderer::init(&win);

	glui::Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	Font* font30 = new Font("arial.ttf", 30);
	if (!font30->inited()) {
		win.destroy();
		std::cout << "Could not load arial.ttf!\n";
		return -1;
	}

	Font* font20 = new Font("arial.ttf", 20);
	if (!font20->inited()) {
		win.destroy();
		std::cout << "Could not load arial.ttf!\n";
		return -1;
	}
    
    Theme theme = {};
    theme.body					= color::lightGrey;
    theme.check					= color::black;
    theme.circle				= color::black;
    theme.hover					= color::grey;
    theme.outline				= color::black;
    theme.press					= color::darkGrey;
    theme.text					= color::black;
    theme.popupBackground		= color::lightGrey;
    theme.popupText				= color::black;
    
    TextStyle buttonText = { 30, font30 };
    TextStyle textBoxText = { 20, font20 };
    ButtonDescriptor desc = { buttonText,
        [&]()->void {
            char** chars = new char*[2];
            chars[0] = (char*)"Cancel";
            chars[1] = (char*)"Ok";
            
            PopupDescriptor pDesc = {};
            pDesc.width = 300;
            pDesc.height = 200;
            pDesc.title = "Popup!";
            pDesc.text = "This is a popup!";
            pDesc.btnNum = 2;
            pDesc.btnText = (const char**)chars;
            pDesc.window = &win;
            pDesc.bodyTextStyle = textBoxText;
            pDesc.buttonTextStyle = buttonText;
            
            int result = Window::popup(pDesc, theme);
            
            std::cout << "popup returned: " << result << "\n";
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
                  []()->void { //Render function
                      glEnable(GL_DEPTH_TEST);
                      glClearColor(0.2f, 0.3f, 0.8f, 1);
                      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                      
                      //gluPerspective(80, 1, 0.1, 1000);
                      
                      /*
                      glPushMatrix();
                      
                      float rotSpeed = 30;
                      
                      glTranslatef(0, 0, -4);
                      glRotatef(glfwGetTime() * rotSpeed / 1, 1, 0, 0);
                      glRotatef(glfwGetTime() * rotSpeed / 2, 0, 1, 0);
                      glRotatef(glfwGetTime() * rotSpeed / 3, 0, 0, 1);
                      
                      glCallList(box);
                      
                      glPopMatrix();
                       */
                  },
                  [](GLPanelMouseData* data)->void {
                      
                  }, theme);
    
    CheckBoxDescriptor checkBoxDesc = { 
        { 30, font30 }, 
        3, 
        false,
        theme
    };
    
    CheckBox checkBox({100, 200, 30, 30}, layout, "toggle", checkBoxDesc);
	while (win.isOpen()) {
		win.poll();

        button.poll();
        textBox.poll();
        textBox2.poll();
        panel.poll();
        checkBox.poll();

		Renderer::clear(color::grey);

		Renderer::beginDraw();
        
        button.render();
        textBox.render();
        textBox2.render();
        panel.render();
        checkBox.render();

		Renderer::endDraw();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}
