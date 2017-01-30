#include <GLUI/GLUI.h>
#include <GLUIExt.h>

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
		chars[0] = "Cancel";
		chars[1] = "Ok";

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

	while (win.isOpen()) {
		win.poll();

		button.poll();

		Renderer::clear(color::grey);

		Renderer::beginDraw();

		button.render();

		Renderer::endDraw();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}