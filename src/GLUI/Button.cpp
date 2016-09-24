#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	Button::Button(Rectangle bounds, Layout* layout, std::string text, ButtonDescriptor desc) :
		GLUIObject(bounds, layout) {
		m_prevDown = false;
		m_down = false;
		m_hovering = false;
		m_text = text;
		m_desc = desc;
	}

	void Button::poll() {
		float posx = input::InputData::mousePosx * m_layout->getScaleX();
		float posy = input::InputData::mousePosy * m_layout->getScaleY();

		bool down = input::InputData::mouseLeftDown;

		m_hovering = posx >= m_bounds.x &&
			posx <= m_bounds.x + m_bounds.w &&
			m_layout->getHeight() - posy >= m_bounds.y &&
			m_layout->getHeight() - posy <= m_bounds.y + m_bounds.h;

		if (down && !m_prevDown) {
			
			if (m_hovering) {
				m_desc.callBack();
				m_down = true;
			}

			m_prevDown = true;
		} else if (!down && m_prevDown) {
			m_down = false;
			m_prevDown = false;
		}
	}

	void Button::render() {
		Color c = m_desc.theme.body;

		if (m_down) {
			c = m_desc.theme.press;
		} else if (m_hovering) {
			c = m_desc.theme.hover;
		}

		glBegin(GL_QUADS);
		if (m_desc.outThick > 0) {
			glColor3f(m_desc.theme.outline.r, m_desc.theme.outline.g, m_desc.theme.outline.b);
			glVertex2f(m_bounds.x - m_desc.outThick, m_bounds.y - m_desc.outThick);
			glVertex2f(m_bounds.x - m_desc.outThick, m_bounds.y + m_bounds.h + m_desc.outThick);
			glVertex2f(m_bounds.x + m_bounds.w + m_desc.outThick, m_bounds.y + m_bounds.h + m_desc.outThick);
			glVertex2f(m_bounds.x + m_bounds.w + m_desc.outThick, m_bounds.y - m_desc.outThick);
		}
		
		glColor3f(c.r, c.g, c.b);
		glVertex2f(m_bounds.x, m_bounds.y);
		glVertex2f(m_bounds.x, m_bounds.y + m_bounds.h);
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y + m_bounds.h);
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y);
		glEnd();

		float off = (m_bounds.h - m_desc.style.size)/2.0f;

		Renderer::drawString(m_text, m_bounds.x + off, m_bounds.y + off*1.5f, m_desc.style.size, m_desc.style.font, &(m_desc.theme.text));
	}
}