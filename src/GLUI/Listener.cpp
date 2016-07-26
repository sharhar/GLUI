#include <GLUI/Listener.h>

namespace glui {
	Listener::Listener(func_check check, func_callBack callBack) {
		m_check = check;
		m_callBack = callBack;
	}

	void Listener::tick() {
		if (m_check()) {
			m_callBack();
		}
	}
}