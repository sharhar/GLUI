#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	CheckBox::CheckBox(Rectangle bounds, Layout* layout, std::string text, CheckBoxDescriptor desc)  :
		GLUIObject(bounds,layout){
		m_text = text;
		m_desc = desc;
		m_checked = desc.checkedAtStart;
		m_prevDown = false;
	}

	void CheckBox::poll() {
		float posx = input::InputData::mousePosx * m_layout->getScaleX();
		float posy = input::InputData::mousePosy * m_layout->getScaleY();

		bool down = input::InputData::mouseLeftDown;

		bool hovering = posx >= m_bounds.x &&
			posx <= m_bounds.x + m_bounds.w &&
			m_layout->getHeight() - posy >= m_bounds.y &&
			m_layout->getHeight() - posy <= m_bounds.y + m_bounds.h;

		if (down && !m_prevDown) {
			if (hovering) {
				m_checked = !m_checked;
			}

			m_prevDown = true;
		} else if (!down && m_prevDown) {
			m_prevDown = false;
		}
	}

	void CheckBox::render() {
		glBegin(GL_QUADS);

		glColor3f(m_desc.theme.outline.r, m_desc.theme.outline.g, m_desc.theme.outline.b);
		
		glVertex2f(m_bounds.x              - m_desc.outLineThickness, m_bounds.y              - m_desc.outLineThickness);
		glVertex2f(m_bounds.x + m_bounds.w + m_desc.outLineThickness, m_bounds.y              - m_desc.outLineThickness);
		glVertex2f(m_bounds.x + m_bounds.w + m_desc.outLineThickness, m_bounds.y + m_bounds.h + m_desc.outLineThickness);
		glVertex2f(m_bounds.x              - m_desc.outLineThickness, m_bounds.y + m_bounds.h + m_desc.outLineThickness);

		glColor3f(m_desc.theme.body.r, m_desc.theme.body.g, m_desc.theme.body.b);
		
		glVertex2f(m_bounds.x             , m_bounds.y             );
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y             );
		glVertex2f(m_bounds.x + m_bounds.w, m_bounds.y + m_bounds.h);
		glVertex2f(m_bounds.x             , m_bounds.y + m_bounds.h);

		glEnd();

		glPushMatrix();

		glTranslatef(m_bounds.x + m_bounds.w/2.0f, m_bounds.y + m_bounds.h/2.0f, 0);

		glRotatef(-30, 0, 0, 1);

		glBegin(GL_QUADS);

		if(m_checked) {
			glColor3f(m_desc.theme.check.r, m_desc.theme.check.g, m_desc.theme.check.b);
		
			glVertex2f(               0     , -m_bounds.h/2.0f + m_bounds.h/15.0f);
			glVertex2f( m_bounds.w/15.0f * 2, -m_bounds.h/2.0f + m_bounds.h/15.0f);
			glVertex2f( m_bounds.w/15.0f * 2,  m_bounds.h/2.0f - m_bounds.h/15.0f);
			glVertex2f(               0     ,  m_bounds.h/2.0f - m_bounds.h/15.0f);

			glVertex2f(-m_bounds.w/15.0f * 3.5f, -m_bounds.h/2.0f + m_bounds.h/15.0f     + m_bounds.h / 20.0f);
			glVertex2f( m_bounds.w/15.0f       , -m_bounds.h/2.0f + m_bounds.h/15.0f                         );
			glVertex2f( m_bounds.w/15.0f       , -m_bounds.h/2.0f + m_bounds.h/15.0f * 3                     );
			glVertex2f(-m_bounds.w/15.0f * 3.5f, -m_bounds.h/2.0f + m_bounds.h/15.0f * 3 + m_bounds.h / 20.0f);
		}

		glEnd();

		glPopMatrix();

		Renderer::drawString(m_text, m_bounds.x + m_bounds.w + m_desc.outLineThickness + 3, m_bounds.y + m_desc.textStyle.size/10.0f, m_desc.textStyle.size, m_desc.textStyle.font, &(m_desc.theme.text));
	}
}