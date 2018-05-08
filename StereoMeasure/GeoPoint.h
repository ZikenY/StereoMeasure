

#pragma once

class CGeoPOINT
{
public:
	double x;
	double y;
	double z;
public:
	CGeoPOINT():x(0),y(0),z(0)
	{}
	CGeoPOINT(double pt_x,double pt_y,double pt_z):x(pt_x),y(pt_y),z(pt_z)
	{}
	CGeoPOINT(const CGeoPOINT& geoPt)
	{
		x = geoPt.x;
		y = geoPt.y;
		z = geoPt.z;
	}
	void operator=(const CGeoPOINT& geoPt)
	{
		x = geoPt.x;
		y = geoPt.y;
		z = geoPt.z;
	}
	void operator-=(const CGeoPOINT& geoPt)
	{
		x -= geoPt.x;
		y -= geoPt.y;
		z -= geoPt.z;
	}
};