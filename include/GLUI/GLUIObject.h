#pragma once

#include <GLUI/Layout.h>

namespace glui {
	class GLUIObject {
	protected:
		Rectangle m_bounds;
		Layout* m_layout;
	public:
		GLUIObject(Rectangle bounds, Layout* layout);

		Rectangle getBounds() { return m_bounds; }
		void setPos(Vector2f pos);
		void setSize(Vector2f size);

		virtual void poll() = 0;
		virtual void render() = 0;
	};
}