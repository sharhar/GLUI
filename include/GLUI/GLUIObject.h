#pragma once

#include <GLUI/Layout.h>

namespace glui {
	class GLUIObject {
	protected:
		Rectangle m_bounds;
		Layout* m_layout;
		int m_windowID;

		virtual void pollFunction() = 0;
	public:
		GLUIObject(Rectangle bounds, Layout* layout, int windowID);

		Rectangle getBounds() { return m_bounds; }
		void setPos(Vector2f pos);
		void setSize(Vector2f size);

		void poll();
		virtual void render() = 0;
	};
}