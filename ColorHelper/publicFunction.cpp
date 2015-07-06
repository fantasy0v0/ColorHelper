#include "stdafx.h"
#include "publicFunction.h"

//将一个十六进制字符串格式化成7个字符显示 
// #FFFFFF
void makeHex(wchar_t* buf) {
	// 最长6个字符
	int len = wcsnlen(buf, 6);
	// 字符串结束
	buf[7] = '\0';
	//将字符串右移
	for (int i = 6, j = len - 1; i >= 0; i--) {
		if (j >= 0)
		{
			buf[i] = buf[j];
			j--;
		}
		else if (i != 0) {
			//空余出的部分补0
			buf[i] = '0';
		}
		else if (i == 0) {
			//开头补#
			buf[i] = '#';
		}
	}
}

//将COLORREF转换成16进制字符串
void getColorHex(COLORREF color, wchar_t* buf) {
	int colorNum = 0, R, G, B;
	R = (color & 0xff) << 16;
	G = color & 0xff00;
	B = (color & 0xff0000) >> 16;
	colorNum = R | G | B;
	_itow(colorNum, buf, 16);
	makeHex(buf);
}

//将120px这种字符串转换成数字
int styleToInt(const wchar_t *strVal) {
	//最后要返回的值
	int value = 0;
	//去掉后面的px
	int strLen = wcslen(strVal) - 2;
	//保存截取后的字符串
	wchar_t tmp[255];
	wcscpy_s(tmp, 255, strVal);
	//设置结束符
	tmp[strLen] = '\0';
	value = _wtoi(tmp);
	return value;
}

//画指定大小的像素点
void drawPixel(HDC hdc, int x, int y, COLORREF color, int multiple) {
	int startX = x * multiple;
	int startY = y * multiple;
	for (int i = 0; i < multiple; i++)
	{
		for (int j = 0; j < multiple; j++)
		{
			SetPixel(hdc, startX + j, startY + i, color);
		}
	}
}