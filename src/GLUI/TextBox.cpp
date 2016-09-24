#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	TextBox::TextBox(Rectangle bounds, Layout* layout, TextBoxDescriptor desc) : 
		GLUIObject(bounds, layout) {
		m_desc = desc;
		m_text = "";
		m_prevDown = false;
		m_writting = false;
		m_isActive = false;
		m_cursorPos = 0;
		m_prevTextSize = 0;
		m_renderCursor = true;
		m_time = 0;

		m_prevKeys = new unsigned char[GLFW_KEY_LAST];
		memset(m_prevKeys, 0, sizeof(unsigned char) * GLFW_KEY_LAST);

		m_enterFunc = []() -> void {};
	}

	void TextBox::clearText() {
		input::keyboard::setTextCallback(NULL);
		input::keyboard::setEnterFunc(NULL);

		m_text = "";
		m_cursorPos = 0;
	}

	TextBox::~TextBox() {
		delete[] m_prevKeys;
	}
	
	void TextBox::setEnterFunc(std::function<void(void)> enterFunc) {
		m_enterFunc = enterFunc;
	}

	void TextBox::poll() {
		float posx = input::InputData::mousePosx * m_layout->getScaleX();
		float posy = input::InputData::mousePosy * m_layout->getScaleY();

		bool down = input::InputData::mouseLeftDown;

		bool hovering = posx >= m_bounds.x &&
			posx <= m_bounds.x + m_bounds.w &&
			m_layout->getHeight() - posy >= m_bounds.y &&
			m_layout->getHeight() - posy <= m_bounds.y + m_bounds.h;

		m_isActive = input::InputData::textString == &m_text;

		if (down && !m_prevDown) {
			if (hovering) {
				input::keyboard::setTextCallback(&m_text);
				input::keyboard::setTextInsertPoint(m_cursorPos);
				input::keyboard::setEnterFunc(&m_enterFunc);
				m_writting = true;
				m_isActive = true;
				m_time = glfwGetTime();
				m_renderCursor = true;
			}
			else {
				if (m_isActive) {
					input::keyboard::setTextCallback(NULL);
					input::keyboard::setEnterFunc(NULL);
				}

				m_renderCursor = true;
				m_writting = false;
			}

			m_prevDown = true;
		} else if (!down && m_prevDown) {
			m_prevDown = false;
		}

		if (m_isActive) {
			input::keyboard::setTextInsertPoint(m_cursorPos);
		}

		size_t sizeOff = m_text.size() - m_prevTextSize;
		m_cursorPos += sizeOff;
		m_prevTextSize = m_text.size();

		if (input::InputData::keys[GLFW_KEY_LEFT] && !m_prevKeys[GLFW_KEY_LEFT] && m_isActive && m_cursorPos > 0) {
			m_cursorPos -= 1;
			m_renderCursor = true;
			m_time = glfwGetTime();
		}

		if (input::InputData::keys[GLFW_KEY_RIGHT] && !m_prevKeys[GLFW_KEY_RIGHT] && m_isActive && m_text.size() > m_cursorPos) {
			m_cursorPos += 1;
			m_renderCursor = true;
			m_time = glfwGetTime();
		}

		if (sizeOff != 0) {
			m_renderCursor = true;
			m_time = glfwGetTime();
		}

		memcpy(m_prevKeys, input::InputData::keys, sizeof(unsigned char)*GLFW_KEY_LAST);

		if (m_isActive && glfwGetTime() - m_time >= 0.55) {
			m_renderCursor = !m_renderCursor;
			m_time = glfwGetTime();
		}
	}

	void TextBox::render() {
		//Get offset of cursor
		Character** chars = (Character**)m_desc.style.font->chars[m_desc.style.font->current];

		float xOff = 0;
		float s = m_desc.style.size / m_desc.style.font->size;

		for (unsigned int i = 0; i < m_text.size(); i++) {
			if (m_cursorPos <= i) {
				break;
			}

			char c = m_text.at(i);

			Character* car = chars[c];

			xOff += (car->advance >> 6) * s;
		}

		float yOff = (m_bounds.h - m_desc.style.size) / 2.0f;

		//render text box
		glBegin(GL_QUADS);
		//Render the outline
		glColor3f(m_desc.theme.outline.r, m_desc.theme.outline.g, m_desc.theme.outline.b);
		glVertex2f(m_bounds.x - m_desc.outLineWidth, m_bounds.y - m_desc.outLineWidth);
		glVertex2f(m_bounds.x + m_bounds.w + m_desc.outLineWidth, m_bounds.y - m_desc.outLineWidth);
		glVertex2f(m_bounds.x + m_bounds.w + m_desc.outLineWidth, m_bounds.y + m_bounds.h + m_desc.outLineWidth);
		glVertex2f(m_bounds.x - m_desc.outLineWidth, m_bounds.y + m_bounds.h + m_desc.outLineWidth);

		//Render the textbox
		glColor3f(m_desc.theme.body.r, m_desc.theme.body.g, m_desc.theme.body.b);
		glVertex2f(m_bounds.x, m_bounds.y);
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y);
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y + m_bounds.h);
		glVertex2f(m_bounds.x, m_bounds.y + m_bounds.h);

		if (m_renderCursor) {
			//Render the cursor
			glColor3f(m_desc.theme.outline.r, m_desc.theme.outline.g, m_desc.theme.outline.b);
			glVertex2f(m_bounds.x + m_desc.outLineWidth + xOff, m_bounds.y + m_desc.outLineWidth);
			glVertex2f(m_bounds.x + m_desc.outLineWidth + m_desc.cursorWidth + xOff, m_bounds.y + m_desc.outLineWidth);
			glVertex2f(m_bounds.x + m_desc.outLineWidth + m_desc.cursorWidth + xOff, m_bounds.y + m_bounds.h - m_desc.outLineWidth);
			glVertex2f(m_bounds.x + m_desc.outLineWidth + xOff, m_bounds.y + m_bounds.h - m_desc.outLineWidth);
		}
		glEnd();
		
		//render text
		Renderer::drawString(m_text, m_bounds.x + m_desc.outLineWidth, m_bounds.y + m_desc.outLineWidth + yOff, m_desc.style.size, m_desc.style.font, &(m_desc.theme.text));
	}
}