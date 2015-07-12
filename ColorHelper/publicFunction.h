#pragma once
#include "stdafx.h"

// 公用函数
//将COLORREF转换成16进制字符串
void getColorHex(COLORREF color, wchar_t* buf);
//将一个十六进制字符串格式化成7个字符显示 
void makeHex(wchar_t* buf);
//将120px这种字符串转换成数字
int styleToInt(const wchar_t *);
//画指定大小的像素点
void drawPixel(HDC hdc, int x, int y, COLORREF color, int multiple, int maxWidth, int maxHeight);