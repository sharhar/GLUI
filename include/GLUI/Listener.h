#pragma once

namespace glui {
	typedef void(*func_callBack)(void);
	typedef bool(*func_check)(void);

	class Listener {
	private:
		func_callBack m_callBack;
		func_check m_check;
	public:
		Listener(func_check check, func_callBack callBack);
		void tick();
	};
}