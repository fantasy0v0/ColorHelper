#pragma once
#include "stdafx.h"

namespace htmlayout {
	//自定义的behavior，当鼠标按住时则不在元素中画出图像。
	//元素属性：
	//isclick 当按下鼠标时为true，否而为false
	//cur 表示要画出的图像的资源号
	class test : public behavior {
		//htmlayout::debug_output_console dc;
	public:
		test();

		virtual void attached(HELEMENT he);
		virtual void detached(HELEMENT he);

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc);
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates);
	};
}