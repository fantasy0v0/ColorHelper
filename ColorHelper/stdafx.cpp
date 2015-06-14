// stdafx.cpp : source file that includes just the standard includes
// ColorHelper.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//将一个十六进制字符串格式化成6个字符显示 
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

void getColorHex(COLORREF color, wchar_t* buf) {
	int colorNum=0, R, G, B;
	R = (color & 0xff) << 16;
	G = color & 0xff00;
	B = (color & 0xff0000) >> 16;
	colorNum = R | G | B;
	_itow(colorNum, buf, 16);
	makeHex(buf);
}