#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	CheckBox::CheckBox(Rectangle bounds, Layout* layout, std::string text, CheckBoxDescriptor desc, int windowID)  :
		GLUIObject(bounds,layout, windowID){
		m_text = text;
		m_desc = desc;
		m_checked = desc.checkedAtStart;
		m_prevDown = false;
	}

	void CheckBox::pollFunction() {
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
        
        
        Renderer::drawRect(m_bounds.x - m_desc.outLineThickness, m_bounds.y - m_desc.outLineThickness,
                           m_bounds.w + m_desc.outLineThickness*2, + m_bounds.h + m_desc.outLineThickness*2,
                           m_desc.theme.outline);
        
        Renderer::drawRect(m_bounds.x, m_bounds.y, m_bounds.w, m_bounds.h, m_desc.theme.body);
        
        if(m_checked) {
            Renderer::drawRect(m_bounds.x + m_bounds.w/2.0f + m_bounds.w/6.0f - m_bounds.w/15.0f,
                               m_bounds.y + m_bounds.h/2.0f,
                               m_bounds.w/15.0f * 2,
                               m_bounds.h - m_bounds.h/15.0f,
                               -30, m_desc.theme.check);
        
            Renderer::drawRect(m_bounds.x + m_bounds.w/2.0f - m_bounds.w/15.0f * 3.5f + m_bounds.w/30.0f*2,
                               m_bounds.y + m_bounds.h/15.0f + m_bounds.h/15.0f * 2,
                               m_bounds.w/15.0f * 4.5f,
                               m_bounds.h / 20.0f - m_bounds.h/15.0f * 3,
                               -30, m_desc.theme.check);
        }

		Renderer::drawString(m_text, m_bounds.x + m_bounds.w + m_desc.outLineThickness + 3,
                             m_bounds.y + m_desc.textStyle.size/10.0f, m_desc.textStyle.size,
                             m_desc.textStyle.font, m_desc.theme.text);
	}
}
