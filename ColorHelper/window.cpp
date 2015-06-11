#include "stdafx.h"
#include "window.h"

#ifdef _DEBUG
htmlayout::debug_output_console _debug_stream;
#endif

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
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDC_COLORHELPER);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = CLASSNAME;
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDC_MYICON);

		return RegisterClassExW(&wcex);
	}

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

	// create window instance

	window* window::create(int x, int y, int width, int height, const wchar_t* caption)
	{
		window* pw = new window();

		UINT style = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
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
			pw->icoFrame = r.get_element_by_id("icoFrame");

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
	//heΪ��Ԫ�أ�targetΪ�������¼���he�µ���Ԫ��
	BOOL window::on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) { 
		if (target == icoFrame && event_type == MOUSE_CLICK)
		{
			icoFrame.update(true);
			dc.printf("MOUSE_CLICK\n");
			return TRUE;
		}
		return FALSE; }
	BOOL window::on_key(HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates) {
		dc.printf("on_key\n");
		return FALSE; }
	BOOL window::on_focus(HELEMENT he, HELEMENT target, UINT event_type) {
		
		return FALSE; }
	BOOL window::on_timer(HELEMENT he) { return FALSE; /*stop this timer*/ }
	BOOL window::on_timer(HELEMENT he, UINT_PTR extTimerId) { return FALSE; /*stop this timer*/ }
	BOOL window::on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
		dc.printf("on_draw\n");
		return true; /*do default draw*/ 
	}
	void window::on_size(HELEMENT he) {
		dc.printf("on_size\n");
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

		switch (message)
		{

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

	LRESULT CALLBACK HTMLayoutNotifyHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam){
		// all HTMLayout notification are comming here.
		NMHDR*  phdr = (NMHDR*)lParam;
		switch (phdr->code)
		{
		case HLN_LOAD_DATA:     break;//return OnLoadData((LPNMHL_LOAD_DATA)lParam);
		case HLN_DATA_LOADED:    break;//return OnDataLoaded((LPNMHL_DATA_LOADED)lParam);
		case HLN_DOCUMENT_LOADED:
			break; //return OnDocumentComplete();
		case HLN_ATTACH_BEHAVIOR: break;//return OnAttachBehavior((LPNMHL_ATTACH_BEHAVIOR)lParam);
		}
		return 0;
	}
}