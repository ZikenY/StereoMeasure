
#pragma once

#include <fstream>


class CImgPOINT
{
public:
	long x;
	long y;
public:
	CImgPOINT():x(0),y(0)
	{}
	CImgPOINT(long pt_x,long pt_y):x(pt_x),y(pt_y)
	{}
	CImgPOINT(const CImgPOINT& imgPt)
	{
		x = imgPt.x;
		y = imgPt.y;
	}
// 	CImgPOINT(const CPoint& imgPt)
// 	{
// 		x = imgPt.x;
// 		y = imgPt.y;
// 	}
};