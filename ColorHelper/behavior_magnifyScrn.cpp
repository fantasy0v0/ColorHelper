#include "stdafx.h"
#include "behavior_manifyScrn.h"
namespace htmlayout {
	//自定义的behavior，画出鼠标所在位置的图像
	//元素属性：
	//isupdate： true表示更新元素的图像为当前鼠标位置的图像，false则使用缓存中的图像
	//multiple： 将图像放大的倍数

	magnifyScrn::magnifyScrn() : behavior(HANDLE_ALL, "magnifyScrn") {

	}

	magnifyScrn::~magnifyScrn() {
		DeleteObject(hBitMap);
	}

	void magnifyScrn::attached(HELEMENT he)
	{
		dom::element el = he;
		//获取元素的大小
		elementWidht = styleToInt(el.get_style_attribute("width"));
		elementHeight = styleToInt(el.get_style_attribute("height"));
		//获得放大倍数
		multiple = _wtoi(el.get_attribute("multiple"));
		//放大前的图像大小
		width = ceil((float)elementWidht / multiple);
		height = ceil((float)elementHeight / multiple);
	}

	void magnifyScrn::detached(HELEMENT he)
	{

	}

	BOOL magnifyScrn::on_draw(HELEMENT he, UINT draw_type, HDC hdc, const RECT& rc) {
		
		dom::element el = he;
		std::wstring str = el.get_attribute("isupdate");
		
		if (L"false" == str){
			dc.printf("使用缓存画面");
			HDC cdc = CreateCompatibleDC(hdc);
			HGDIOBJ oldBitmap = SelectObject(cdc, hBitMap);
			// +1 是因为边框的原因
			BitBlt(hdc, rc.left + 1, rc.top + 1, elementWidht - 1, elementHeight - 1, cdc, 0, 0, SRCCOPY);
			SelectObject(cdc, oldBitmap);
			DeleteDC(cdc);
			return false;
		}

		//获得分辨率
		int ScrWidth = GetDeviceCaps(hdc, HORZRES);
		int ScrHeight = GetDeviceCaps(hdc, VERTRES);
		//整个屏幕的DC，用于获取图像
		HDC fullDC = GetDC(nullptr);
		HBITMAP fullBitMap = CreateCompatibleBitmap(fullDC, ScrWidth, ScrHeight);
		HDC fullcdc = CreateCompatibleDC(fullDC);
		HGDIOBJ oldFullBitmap = SelectObject(fullcdc, fullBitMap);
		//将整个屏幕的DC复制到兼容DC上
		BitBlt(fullcdc, 0, 0, ScrWidth, ScrHeight, fullDC, 0, 0, SRCCOPY);
		//元素的兼容DC
		if (false == iscreate) {
			hBitMap = CreateCompatibleBitmap(hdc, elementWidht + 20, elementHeight + 20);
		}
		HDC cdc = CreateCompatibleDC(hdc);
		HGDIOBJ oldBitmap = SelectObject(cdc, hBitMap);
		//获得鼠标位置
		POINT pt;
		GetCursorPos(&pt);
		//存储要取的图像的范围
		RECT rect;
		rect.left = pt.x - (width / 2);
		rect.top = pt.y - (height / 2);
		rect.right = pt.x + (width / 2);
		rect.bottom = pt.y + (height / 2);

		//将全屏的DC拷贝到内存中再进行GetPixel，否者会很慢！！！
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				COLORREF color = GetPixel(fullcdc, rect.left + j, rect.top + i);
				drawPixel(cdc, j, i, color, multiple, elementWidht, elementHeight);
			}
		}

		// +1 是因为边框的原因
		BitBlt(hdc, rc.left + 1, rc.top + 1, elementWidht - 1, elementHeight - 1, cdc, 0, 0, SRCCOPY);

		//释放全屏的兼容DC
		SelectObject(fullcdc, oldFullBitmap);
		DeleteDC(fullcdc);
		DeleteObject(fullBitMap);
		ReleaseDC(nullptr, fullDC);
		//释放程序的兼容DC
		SelectObject(cdc, oldBitmap);
		DeleteDC(cdc);
		return false;
	}

	BOOL magnifyScrn::on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) {
		return false;
	}

	magnifyScrn magnifyScrn_instance;
}