
#include "stdafx.h" 
#include "window.h"
#include "publicFunction.h"
#include "behavior_test.h"
#include "behavior_manifyScrn.h"
#include "behavior_test.h"
namespace htmlayout
{

	HINSTANCE window::hinstance = 0;

	const wchar_t CLASSNAME[] = L"htmlayout::window";

	// register window class

	ATOM  window::register_class(HINSTANCE hInstance)
	{
		hinstance = hInstance;

		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)win_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = CLASSNAME;
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);

		return RegisterClassExW(&wcex);
	}

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

	// create window instance

	window* window::create(int x, int y, int width, int height, const wchar_t* caption)
	{
		window* pw = new window();

		UINT style = WS_POPUP ;
		pw->hwnd = CreateWindowExW(WS_EX_LAYERED, CLASSNAME, NULL, style,
			x, y, width, height, NULL, NULL, hinstance, NULL);
		//    pw->hwnd = CreateWindowExW( 0, CLASSNAME, NULL, style ,
		//                                x, y, width, height, NULL, NULL, hinstance, NULL);
		self(pw->hwnd, pw);
		HTMLayoutSetCallback(pw->hwnd, &callback, pw);
		PBYTE pb; DWORD cb;
		if (load_resource_data(L"DEFAULT", pb, cb))
		{
			HTMLayoutLoadHtml(pw->hwnd, pb, cb);

			dom::element r = pw->root();

			pw->body = r.find_first("body");
			pw->caption = r.get_element_by_id("caption");

			pw->button_icon = r.get_element_by_id("icon");
			pw->button_close = r.get_element_by_id("close");
			pw->pickupFrame = r.get_element_by_id("pickupFrame");
			pw->colorFrame = r.get_element_by_id("colorFrame");
			pw->r = r.get_element_by_id("r");
			pw->g = r.get_element_by_id("g");
			pw->b = r.get_element_by_id("b");
			pw->rgbHex = r.get_element_by_id("rgbHex");
			attach_event_handler(pw->hwnd, pw);

			pw->set_caption(caption);

		}
		return pw;
	}

	void window::set_caption(const wchar_t* text)
	{
		if (text)
		{
			::SetWindowTextW(hwnd, text);
			if (caption.is_valid())
			{
				caption.set_text(text);
				//caption.update(true);
			}
		}
	}


	HELEMENT  window::root()
	{
		return dom::element::root_element(hwnd);
	}

	int       window::hit_test(int x, int y)
	{

		POINT pt; pt.x = x; pt.y = y;
		::MapWindowPoints(HWND_DESKTOP, hwnd, &pt, 1);

		if (caption.is_valid() && caption.is_inside(pt))
			return HTCAPTION;

		if (button_icon.is_valid() && button_icon.is_inside(pt))
			return HTSYSMENU;


		RECT body_rc = body.get_location(ROOT_RELATIVE | CONTENT_BOX);

		if (PtInRect(&body_rc, pt))
			return HTCLIENT;

		if (pt.y < body_rc.top + 10)
		{
			if (pt.x < body_rc.left + 10)
				return HTTOPLEFT;
			if (pt.x > body_rc.right - 10)
				return HTTOPRIGHT;
		}
		else if (pt.y > body_rc.bottom - 10)
		{
			if (pt.x < body_rc.left + 10)
				return HTBOTTOMLEFT;
			if (pt.x > body_rc.right - 10)
			{
				
				dc.printf("HTBOTTOMRIGHT\n");

				return HTBOTTOMRIGHT;
			}
		}

		if (pt.y < body_rc.top)
			return HTTOP;
		if (pt.y > body_rc.bottom)
			return HTBOTTOM;
		if (pt.x < body_rc.left)
			return HTLEFT;
		if (pt.x > body_rc.right)
			return HTRIGHT;


		return HTCLIENT;

	}

	BOOL window::on_event(HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason)
	{
		if (type != BUTTON_CLICK)
			return FALSE; // handling only button clicks here. 

		if (target == button_close)
		{
			::PostMessage(hwnd, WM_CLOSE, 0, 0);
			return TRUE;
		}

		// click on some other button
		dom::element button = target;
		//::MessageBoxW(button.get_element_hwnd(true) ,button.get_attribute("id"), L"Click on the button with id:", MB_OK);
		//::MessageBox(button.get_element_hwnd(true) ,"test", "Click on the button with id:", MB_OK);

		return TRUE;
	}
	//target为发生该事件的元素，he为该元素的父元素
	BOOL window::on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) { 
		if (target == pickupFrame && event_type == MOUSE_CLICK)
		{
			dc.printf("MOUSE_CLICK\n");
			return TRUE;
		}
		if (target == pickupFrame && event_type == MOUSE_DOWN) {
			return pickupFrame_mouse_down();
		}
		if (target == pickupFrame && event_type == MOUSE_UP) {
			return pickupFrame_mouse_up();
		}
		if (target == pickupFrame && event_type == MOUSE_MOVE) {
			return pickupFrame_mouse_move();
		}
		return FALSE; }
	BOOL window::on_key(HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates) {
		return FALSE; }
	BOOL window::on_focus(HELEMENT he, HELEMENT target, UINT event_type) {
		
		return FALSE; }
	BOOL window::on_timer(HELEMENT he) { return FALSE; /*stop this timer*/ }
	BOOL window::on_timer(HELEMENT he, UINT_PTR extTimerId) { return FALSE; /*stop this timer*/ }
	BOOL window::on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
		return FALSE; /*do default draw*/ 
	}
	void window::on_size(HELEMENT he) {
	}

	LRESULT CALLBACK window::win_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult;
		BOOL    bHandled;

		// HTMLayout +
		// HTMLayout could be created as separate window 
		// using CreateWindow API.
		// But in this case we are attaching HTMLayout functionality
		// to the existing window delegating windows message handling to 
		// HTMLayoutProcND function.
		lResult = HTMLayoutProcND(hwnd, message, wParam, lParam, &bHandled);
		if (bHandled)
			return lResult;
		// HTMLayout -

		window* me = self(hwnd);

		switch (message) {
		case WM_NCHITTEST:
			if (me)
				return me->hit_test(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_NCCALCSIZE:  return 0; // we have no non-client areas.
		case WM_NCPAINT:     return 0; // we have no non-client areas.
		case WM_NCACTIVATE:  return (wParam == 0) ? TRUE : FALSE; // we have no non-client areas.

		case WM_GETMINMAXINFO:
		{
			LRESULT lr = DefWindowProcW(hwnd, message, wParam, lParam);
			MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
			pmmi->ptMinTrackSize.x = ::HTMLayoutGetMinWidth(hwnd);
			RECT rc; GetWindowRect(hwnd, &rc);
			pmmi->ptMinTrackSize.y = ::HTMLayoutGetMinHeight(hwnd, rc.right - rc.left);
			return lr;
		}

		case WM_CLOSE:
			::DestroyWindow(hwnd);
			return 0;

		case WM_DESTROY:
			delete me; // delete window instance!
			self(hwnd, 0);
			PostQuitMessage(0);
			return 0;

		}
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}

	//返回true则不将该消息发送给父元素
	BOOL window::pickupFrame_mouse_move() {
		HDC hdc;
		COLORREF color;
		wchar_t colorHex[255];
		POINT pt;
		if (isclick) {
			hdc = GetDC(nullptr);
			GetCursorPos(&pt);
			color = GetPixel(hdc, pt.x, pt.y);
			getColorHex(color, colorHex);
			colorFrame.set_style_attribute("background-color", colorHex);
			setRGBElement(color);
			ReleaseDC(nullptr, hdc);
		}

		return TRUE;
	}

	BOOL window::pickupFrame_mouse_down() {
		isclick = true;
		return TRUE;
	}

	BOOL window::pickupFrame_mouse_up() {
		isclick = false;
		pickupFrame.update(true);
		return TRUE;
	}
	void window::setRGBElement(COLORREF color) {
		wchar_t tmp[10];
		int colorNum;
		// R
		colorNum = color & 0xff;
		_itow(colorNum, tmp, 10);
		r.set_text(tmp);
		// G
		colorNum = (color & 0xff00) >> 8;
		_itow(colorNum, tmp, 10);
		g.set_text(tmp);
		// B
		colorNum = (color & 0xff0000) >> 16;
		_itow(colorNum, tmp, 10);
		b.set_text(tmp);

		// Hex
		getColorHex(color, tmp);
		rgbHex.set_text(tmp);
	}
}