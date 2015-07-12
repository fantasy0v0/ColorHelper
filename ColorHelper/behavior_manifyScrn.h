#pragma once
#include "stdafx.h"

namespace htmlayout {
	//自定义的behavior，画出鼠标所在位置的图像
	//元素属性：
	//update true表示更新元素的图像为当前鼠标位置的图像，false则使用缓存中的图像
	//multiple 将图像放大的倍数
	class magnifyScrn : public behavior {
	private:
		htmlayout::debug_output_console dc;
		int elementWidht;
		int elementHeight;
		int multiple;
		//放大前的图像大小
		int width;
		int height;
		//缓存位图
		HBITMAP hBitMap;
		bool iscreate = false;
	public:
		magnifyScrn();
		~magnifyScrn();

		virtual void attached(HELEMENT he);
		virtual void detached(HELEMENT he);

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc);
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates);
	};
}