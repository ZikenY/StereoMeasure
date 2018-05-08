#pragma once

class CPtPare
{
public:
	CImgPOINT imgPt_L;
	CImgPOINT imgPt_R;
	CGeoPOINT geoPt;
	CPtPare   *next;
public:
	CPtPare(CImgPOINT pt_imgL, CImgPOINT pt_imgR):imgPt_L(pt_imgL),imgPt_R(pt_imgR),geoPt(0,0,0),next(0)
	{}
	CPtPare(CImgPOINT pt_img,CImgPOINT pt_imgR,CGeoPOINT pt_geo):imgPt_L(pt_img),imgPt_R(pt_imgR),geoPt(pt_geo),next(0)
	{}
	CPtPare(const CPtPare& pt_pare)
	{
		imgPt_L = pt_pare.imgPt_L;
		imgPt_R = pt_pare.imgPt_R;
		geoPt = pt_pare.geoPt;
		next = pt_pare.next;
	}
	CPtPare():imgPt_L(0,0),imgPt_R(0,0),geoPt(0,0,0),next(0)
	{}
};