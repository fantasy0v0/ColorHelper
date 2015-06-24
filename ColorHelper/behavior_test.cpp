#include "stdafx.h"

namespace htmlayout {
	//自定义的behavior
	class test : public behavior {
	public:
		test() : behavior(HANDLE_ALL, "test") {

		}
		htmlayout::debug_output_console dc;

		virtual void attached(HELEMENT he)
		{
			
		}

		virtual void detached(HELEMENT he)
		{
			
		}

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
			//要被重绘的元素
			dom::element el = he;
			//最后要释放的对象
			HDC hcdc;
			HBITMAP hbm;
			HGDIOBJ old_obj;
			//设置这个元素的左上角坐标
			RECT rect = rc;
			//位图信息
			BITMAP bitmap;

			std::wstring str = el.get_attribute("isclick");
			if (_T("true") == str){
				return false;
			}
			//获得元素的cur属性，来载入相应的位图
			str = el.get_attribute("cur");
			int curId = _wtoi(str.c_str());
			hbm = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(curId));
			GetObject(hbm, sizeof(BITMAP), &bitmap);
			//创建兼容DC
			hcdc = CreateCompatibleDC(hdc);
			//选入位图
			old_obj = SelectObject(hcdc, hbm);
			//将矩形居中，来画出位图
			rect.left += (rc.right - rc.left) / 2 - bitmap.bmWidth / 2;
			rect.top += (rc.bottom - rc.top) / 2 - bitmap.bmHeight / 2;
			//将兼容DC的内容画到hdc上
			TransparentBlt(hdc, rect.left, rect.top, bitmap.bmWidth, bitmap.bmHeight, hcdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));
			//还原旧的位图
			SelectObject(hcdc, old_obj);
			//释放资源
			DeleteObject(hbm);
			DeleteObject(hcdc);
			return false; /*do default draw*/
		}
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
			switch (event_type) {
				case MOUSE_DOWN:
					switch (mouseButtons) {
						case MAIN_MOUSE_BUTTON:
							mouse_left_down(he);
							break;
						default:
							break;
					}
					break;
				case MOUSE_UP:
					switch (mouseButtons) {
						case MAIN_MOUSE_BUTTON:
							mouse_left_UP(he);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			//
			return FALSE;
		}
	private:
		void mouse_left_down(dom::element el) {
			el.set_attribute("isclick", _T("true"));
			el.set_capture();
			el.update(true);
		}
		void mouse_left_UP(dom::element el) {
			el.set_attribute("isclick", _T("false"));
			el.release_capture();
			el.update(true);
		}
	};
	test test_instance;
}