#pragma once
#include "stdafx.h" 

namespace htmlayout
{
	class window : public event_handler,
		public notification_handler<window>
	{
	public:
		HWND          hwnd;
		htmlayout::debug_output_console dc;

		dom::element  body;
		//标题
		dom::element  caption;
		//标题图标
		dom::element  button_icon;
		//窗口关闭
		dom::element  button_close;
		//取色框
		dom::element  pickupFrame;
		//颜色框
		dom::element  colorFrame;
		dom::element  r, g, b;
		//显示16进制的颜色值
		dom::element rgbHex;
		//放大框
		dom::element magnifyFrame;
		dom::element magnifyScrn;

		static  window* create(int x, int y, int width, int height, const wchar_t* caption = 0);
		static  window* self(HWND hWnd) { return (window*)::GetWindowLongPtr(hWnd, GWLP_USERDATA); }

		void            set_caption(const wchar_t* text);

		static  ATOM              register_class(HINSTANCE hInstance);
	protected:
		window() : event_handler(HANDLE_ALL) {}

		int       hit_test(int x, int y);
		HELEMENT  root();
		bool      is_minimized() const;
		bool      is_maximized() const;

		virtual BOOL  on_event(HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason);
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates);
		virtual BOOL on_key(HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates);
		virtual BOOL on_focus(HELEMENT he, HELEMENT target, UINT event_type);
		virtual BOOL on_timer(HELEMENT he);
		virtual BOOL on_timer(HELEMENT he, UINT_PTR extTimerId);
		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc);
		virtual void on_size(HELEMENT he);

		static  void              self(HWND hWnd, window* inst) { ::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(inst)); }

		static  LRESULT CALLBACK  win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static  HINSTANCE         hinstance;

	private:
		//取色器框的鼠标事件
		BOOL pickupFrame_mouse_move();
		BOOL pickupFrame_mouse_down();
		BOOL pickupFrame_mouse_up();
		//放大框的鼠标事件
		BOOL magnifyFrame_mouse_move();
		BOOL magnifyFrame_mouse_down();
		BOOL magnifyFrame_mouse_up();
		//放大框
		BOOL magnifyScrn_mouse_click();
		void setRGBElement(COLORREF color);
		bool isclick = false;
		bool isUpdate = false;
	};


}