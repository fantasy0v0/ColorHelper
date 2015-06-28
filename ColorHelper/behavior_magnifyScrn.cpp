#include "stdafx.h"

namespace htmlayout {
	//自定义的behavior
	class magnifyScrn : public behavior {
	public:
		magnifyScrn() : behavior(HANDLE_ALL, "magnifyScrn") {

		}
		htmlayout::debug_output_console dc;

		virtual void attached(HELEMENT he)
		{

		}

		virtual void detached(HELEMENT he)
		{

		}

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
			return false; /*do default draw*/
		}
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
			return FALSE;
		}
	};
	magnifyScrn magnifyScrn_instance;
}