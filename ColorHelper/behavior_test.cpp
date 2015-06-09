#include "stdafx.h"

namespace htmlayout {
	class test : public behavior {
	public:
		test() : behavior(HANDLE_ALL, "test") {

		}
		htmlayout::debug_output_console dc;
		dom::element el;
		bool isclick = false;
		virtual void attached(HELEMENT he)
		{
			dc.printf("test attached\n");
			el = he;
			el.update(true);
		}

		virtual void detached(HELEMENT he)
		{
			dc.printf("test detached\n");
		}

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
			dc.printf("test on_draw\n");
			MoveToEx(hdc, rc.left, rc.top, nullptr);
			LineTo(hdc, rc.right, rc.bottom);
			return false; /*do default draw*/
		}
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
			switch (event_type) {
				case MOUSE_CLICK:
					switch (mouseButtons) {
						case MAIN_MOUSE_BUTTON:
							dc.printf("left click\n");
							break;
						case PROP_MOUSE_BUTTON:
							dc.printf("right click\n");
							break;
						default:
							break;
					}
					break;
				case MOUSE_DOWN:
					switch (mouseButtons) {
						case MAIN_MOUSE_BUTTON:
							mouse_left_down();
							break;
						case PROP_MOUSE_BUTTON:
							dc.printf("right click\n");
							break;
						default:
							break;
					}
					break;
				case MOUSE_UP:
					switch (mouseButtons) {
						case MAIN_MOUSE_BUTTON:
							mouse_left_UP();
							break;
						case PROP_MOUSE_BUTTON:
							
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			return FALSE;
		}
	private:
		void mouse_left_down() {
			dc.printf("left DOWN\n");
			HTMLayoutSetCapture(el);
		}
		void mouse_left_UP() {
			dc.printf("left up\n");
			ReleaseCapture();
		}
	};
	test test_instance;
}