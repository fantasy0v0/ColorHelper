#include "stdafx.h"
#include "behavior_manifyScrn.h"
namespace htmlayout {
	//自定义的behavior，画出鼠标所在位置的图像
	//元素属性：
	//update true表示更新元素的图像为当前鼠标位置的图像，false则使用缓存中的图像
	//multiple 将图像放大的倍数

	magnifyScrn::magnifyScrn() : behavior(HANDLE_ALL, "magnifyScrn") {

	}

	void magnifyScrn::attached(HELEMENT he)
	{
		dom::element el = he;
		//获取元素的大小
		elementWidht = styleToInt(el.get_style_attribute("width"));
		elementHeight = styleToInt(el.get_style_attribute("height"));
		multiple = _wtoi(el.get_attribute("multiple"));
		//放大前的图像大小
		width = ceil(elementWidht / multiple);
		height = ceil(elementHeight / multiple);
	}

	void magnifyScrn::detached(HELEMENT he)
	{

	}

	BOOL magnifyScrn::on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
		
		//整个屏幕的DC，用于获取图像
		HDC fullDC = GetDC(nullptr);
		//获得鼠标位置
		POINT pt;
		GetCursorPos(&pt);
		//存储要取的图像的范围
		RECT rect;
		rect.left = pt.x - (width / 2);
		rect.top = pt.y - (height / 2);
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		//将全屏的DC拷贝到内存中再进行GetPixel，否者会很慢！！！


		//创建兼容位图和兼容DC
		HBITMAP hBitMap = CreateCompatibleBitmap(fullDC, width, height);
		HDC cdc = CreateCompatibleDC(fullDC);
		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				COLORREF color = GetPixel(fullDC, rect.left + j, rect.top + i);
				drawPixel(cdc, j, i, color, multiple);
			}
		}
		//释放资源
		DeleteDC(cdc);
		DeleteObject(hBitMap);
		ReleaseDC(nullptr, fullDC);
		return true; /*do default draw*/
	}

	BOOL magnifyScrn::on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
		return false;
	}

	magnifyScrn magnifyScrn_instance;
	/*
	class magnifyScrn : public behavior {
	public:
		magnifyScrn::magnifyScrn() : behavior(HANDLE_ALL, "magnifyScrn") {

		}
		htmlayout::debug_output_console dc;

		virtual void attached(HELEMENT he)
		{
			
		}

		virtual void detached(HELEMENT he)
		{
			
		}

		virtual BOOL on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
			dom::element el = he;
			//获取元素的大小
			int elementWidht = styleToInt(el.get_style_attribute("width"));
			int elementHeight = styleToInt(el.get_style_attribute("height"));
			int multiple = _wtoi(el.get_attribute("multiple"));
			//放大前的图像大小
			int width = ceil(elementWidht / multiple);
			int height = ceil(elementHeight / multiple);
			//整个屏幕的DC，用于获取图像
			HDC fullDC = GetDC(nullptr);
			//创建兼容位图和兼容DC
			HBITMAP hBitMap = CreateCompatibleBitmap(fullDC, width, height);
			HDC cdc = CreateCompatibleDC(fullDC);
			//获得鼠标位置
			POINT pt;
			GetCursorPos(&pt);
			//存储要取的图像的范围
			RECT rect;
			rect.left = pt.x - (width / 2);
			rect.top = pt.y - (height / 2);
			rect.right = rect.left + width;
			rect.bottom = rect.top + height;
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					COLORREF color = GetPixel(fullDC, rect.left + j, rect.top + i);
					drawPixel(cdc, j, i, color, multiple);
				}
			}
			//释放资源
			DeleteDC(cdc);
			DeleteObject(hBitMap);
			ReleaseDC(nullptr, fullDC);
			return true; //do default draw
		}
		virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
			return false;
		}
	};
	*/
}