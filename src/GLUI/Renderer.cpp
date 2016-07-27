#include <GLUI/Renderer.h>
#include <GLUI/DONOTINCLUDE.h>

namespace glui {
	void Renderer::render(const std::string& text, float posx, float posy, float scale, Font* font, Color* color) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		Character** chars = (Character**)font->chars;

		float xOff = 0;

		glColor3f(color->r, color->g, color->b);
		for (int i = 0; i < text.size();i++) {
			char c = text.at(i);

			Character* car = chars[c];

			GLuint tex = car->tex;
			
			glBindTexture(GL_TEXTURE_2D, tex);

			float h = car->sizey * scale;
			float w = car->sizex * scale;
			
			float x = xOff + posx + car->offx * scale;
			float y = posy - (car->sizey - car->offy) * scale;

			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex2f(x    , y    );
			glTexCoord2d(0.0, 0.0); glVertex2f(x    , y + h);
			glTexCoord2d(1.0, 0.0); glVertex2f(x + w, y + h);
			glTexCoord2d(1.0, 1.0); glVertex2f(x + w, y    );
			glEnd();

			xOff += (car->advance >> 6) * scale;
		}
	}
}