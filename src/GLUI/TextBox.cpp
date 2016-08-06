#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	TextBox::TextBox(Rectangle rect, TextBoxDescriptor desc) {
		m_desc = desc;
		m_rect = rect;
		m_text = "";
		m_prevDown = false;
		m_writting = false;
	}

	void TextBox::poll() {
		float posx = input::Input::input->mousePosx * m_desc.layout->getScaleX();
		float posy = input::Input::input->mousePosy * m_desc.layout->getScaleY();

		bool down = input::Input::input->mouseLeftDown;

		bool hovering = posx >= m_rect.x &&
			posx <= m_rect.x + m_rect.w &&
			m_desc.layout->getHeight() - posy >= m_rect.y &&
			m_desc.layout->getHeight() - posy <= m_rect.y + m_rect.h;

		if (down && !m_prevDown) {
			if (hovering) {
				input::keyboard::setTextCallback(&m_text);
				m_writting = true;
			} else {
				if (input::Input::textString == &m_text) {
					input::keyboard::setTextCallback(0);
				}
				
				m_writting = false;
			}

			m_prevDown = true;
		} else if (!down && m_prevDown) {
			m_prevDown = false;
		}
	}

	void TextBox::render() {
		//Get offset of cursor
		Character** chars = (Character**)m_desc.style.font->chars;

		float xOff = 0;
		float s = m_desc.style.size / m_desc.style.font->size;

		for (unsigned int i = 0; i < m_text.size(); i++) {
			char c = m_text.at(i);

			Character* car = chars[c];

			xOff += (car->advance >> 6) * s;
		}

		float yOff = (m_rect.h - m_desc.style.size) / 2.0f;

		//render text box
		glBegin(GL_QUADS);
		glColor3f(m_desc.outlineColor.r, m_desc.outlineColor.g, m_desc.outlineColor.b);
		glVertex2f(m_rect.x - m_desc.outLineWidth, m_rect.y - m_desc.outLineWidth);
		glVertex2f(m_rect.x + m_rect.w + m_desc.outLineWidth, m_rect.y - m_desc.outLineWidth);
		glVertex2f(m_rect.x + m_rect.w + m_desc.outLineWidth, m_rect.y + m_rect.h + m_desc.outLineWidth);
		glVertex2f(m_rect.x - m_desc.outLineWidth, m_rect.y + m_rect.h + m_desc.outLineWidth);

		glColor3f(m_desc.boxColor.r, m_desc.boxColor.g, m_desc.boxColor.b);
		glVertex2f(m_rect.x, m_rect.y);
		glVertex2f(m_rect.x + m_rect.w, m_rect.y);
		glVertex2f(m_rect.x + m_rect.w, m_rect.y + m_rect.h);
		glVertex2f(m_rect.x, m_rect.y + m_rect.h);

		glColor3f(m_desc.cursorColor.r, m_desc.cursorColor.g, m_desc.cursorColor.b);
		glVertex2f(m_rect.x + m_desc.outLineWidth + xOff,		m_rect.y + m_desc.outLineWidth);
		glVertex2f(m_rect.x + m_desc.outLineWidth + m_desc.cursorWidth + xOff,		m_rect.y + m_desc.outLineWidth);
		glVertex2f(m_rect.x + m_desc.outLineWidth + m_desc.cursorWidth + xOff,		m_rect.y + m_rect.h - m_desc.outLineWidth);
		glVertex2f(m_rect.x + m_desc.outLineWidth + xOff,		m_rect.y + m_rect.h - m_desc.outLineWidth);
		glEnd();

		
		//render text
		Renderer::drawString(m_text, m_rect.x + m_desc.outLineWidth, m_rect.y + m_desc.outLineWidth + yOff, m_desc.style.size, m_desc.style.font, &(m_desc.style.color));
	}
}