#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	unsigned int Renderer::glInitList = 0;

	void Renderer::init(Window* window) {
		glInitList = glGenLists(1);
		glNewList(glInitList, GL_COMPILE_AND_EXECUTE);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, window->getWidth(), 0, window->getHeight(), -1, 1);
		glMatrixMode(GL_MODELVIEW);

		glViewport(0, 0, window->getWidth(), window->getHeight());

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MULTISAMPLE);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEndList();
	}

	void Renderer::reinit() {
		glCallList(glInitList);
	}

	void Renderer::clear(Color color) {
		glClearColor(color.r, color.g, color.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::drawString(const std::string& text, float posx, float posy, float scale, Font* font, Color* color) {
		drawString(text, font->current, posx, posy, scale, font, color);
	}

	void Renderer::drawString(const std::string& text, int num, float posx, float posy, float scale, Font* font, Color* color) {
		Character** chars = (Character**)font->chars[num];

		float xOff = 0;
		float yOff = 0;

		float s = scale/font->size;

		glColor3f(color->r, color->g, color->b);
		for (unsigned int i = 0; i < text.size();i++) {
			char c = text.at(i);

			if (c == '\n') {
				yOff += scale;
				xOff = 0;
				continue;
			}

			Character* car = chars[c];

			GLuint tex = car->tex;
			
			glBindTexture(GL_TEXTURE_2D, tex);

			float h = car->sizey * s;
			float w = car->sizex * s;
			
			float x = xOff + posx + car->offx * s;
			float y = posy - (car->sizey - car->offy) * s - yOff;

			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex2f(x    , y    );
			glTexCoord2d(0.0, 0.0); glVertex2f(x    , y + h);
			glTexCoord2d(1.0, 0.0); glVertex2f(x + w, y + h);
			glTexCoord2d(1.0, 1.0); glVertex2f(x + w, y    );
			glEnd();

			xOff += (car->advance >> 6) * s;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}