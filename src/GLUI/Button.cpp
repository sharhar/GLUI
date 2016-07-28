#include <GLUI/Button.h>
#include <GLUI/DONOTINCLUDE.h>
#include <GLUI/Input.h>
#include <GLUI/Renderer.h>

namespace glui {
	Button::Button(Rectangle rect, std::string text, BasicButtonDescriptor desc) :
		m_rect(rect) {
		m_prevDown = false;
		m_down = false;
		m_hovering = false;
		m_text = text;

		float outColor = 0.45f;
		float nt = 0.6f;
		float nd = 0.4f;
		float ht = 0.55f;
		float hd = 0.35f;
		float dt = 0.2f;
		float dd = 0.42f;
		
		m_descriptor.basic = desc;
		m_descriptor.out = true;
		m_descriptor.outThick = 3;
		m_descriptor.outColor = { 0.45f, 0.45f, 0.45f };

		m_descriptor.ntr = { nt, nt, nt };
		m_descriptor.ntl = { nt, nt, nt };
		m_descriptor.nbr = { nd, nd, nd };
		m_descriptor.nbl = { nd, nd, nd };

		m_descriptor.htr = { ht, ht, ht };
		m_descriptor.htl = { ht, ht, ht };
		m_descriptor.hbr = { hd, hd, hd };
		m_descriptor.hbl = { hd, hd, hd };

		m_descriptor.dtr = { dt, dt, dt };
		m_descriptor.dtl = { dt, dt, dt };
		m_descriptor.dbr = { dd, dd, dd };
		m_descriptor.dbl = { dd, dd, dd };
	}

	void Button::poll() {
		float posx = input::Input::input->mousePosx * m_descriptor.basic.layout->getScaleX();
		float posy = input::Input::input->mousePosy * m_descriptor.basic.layout->getScaleY();

		bool down = input::Input::input->mouseLeftDown;

		m_hovering = posx >= m_rect.x &&
			posx <= m_rect.x + m_rect.w &&
			m_descriptor.basic.layout->getHeight() - posy >= m_rect.y &&
			m_descriptor.basic.layout->getHeight() - posy <= m_rect.y + m_rect.h;

		if (down && !m_prevDown) {
			
			if (m_hovering) {
				m_descriptor.basic.callBack();
				m_down = true;
			}

			m_prevDown = true;
		} else if (!down && m_prevDown) {
			m_down = false;
			m_prevDown = false;
		}
	}

	void Button::render() {
		Color tr = m_descriptor.ntr;
		Color tl = m_descriptor.ntl;
		Color br = m_descriptor.nbr;
		Color bl = m_descriptor.nbl;

		if (m_down) {
			tr = m_descriptor.dtr;
			tl = m_descriptor.dtl;
			br = m_descriptor.dbr;
			bl = m_descriptor.dbl;
		} else if (m_hovering) {
			tr = m_descriptor.htr;
			tl = m_descriptor.htl;
			br = m_descriptor.hbr;
			bl = m_descriptor.hbl;
		}

		glBegin(GL_QUADS);
		if (m_descriptor.out) {
			glColor3f(m_descriptor.outColor.r, m_descriptor.outColor.g, m_descriptor.outColor.b);
			glVertex2f(m_rect.x - m_descriptor.outThick, m_rect.y - m_descriptor.outThick);
			glVertex2f(m_rect.x - m_descriptor.outThick, m_rect.y + m_rect.h + m_descriptor.outThick);
			glVertex2f(m_rect.x + m_rect.w + m_descriptor.outThick, m_rect.y + m_rect.h + m_descriptor.outThick);
			glVertex2f(m_rect.x + m_rect.w + m_descriptor.outThick, m_rect.y - m_descriptor.outThick);
		}
		
		glColor3f(bl.r, bl.g, bl.b);
		glVertex2f(m_rect.x, m_rect.y);
		glColor3f(tl.r, tl.g, tl.b);
		glVertex2f(m_rect.x, m_rect.y + m_rect.h);
		glColor3f(tr.r, tr.g, tr.b);
		glVertex2f(m_rect.x + m_rect.w, m_rect.y + m_rect.h);
		glColor3f(br.r, br.g, br.b);
		glVertex2f(m_rect.x + m_rect.w, m_rect.y);
		glEnd();

		float off = (m_rect.h - m_descriptor.basic.style.size)/2.0f;

		Renderer::drawString(m_text, m_rect.x + off, m_rect.y + off*1.5f, m_descriptor.basic.style.size, m_descriptor.basic.style.font, &(m_descriptor.basic.style.color));
	}
}