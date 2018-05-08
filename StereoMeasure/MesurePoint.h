#pragma once
#include "GeoPoint.h"
#include "ImgPoint.h"
#include "PtPare.h"
#include <string>

using namespace  std;

class CMeasurePoint
{
public:
	CPtPare *head;
	int numPoint;//坐标点个数
	double  testValue;//测量值
	int measureWay;//测量方式：0：标记；1：点到点；2：点到线；3：点到面；4：角度；5：面积

public:
	CMeasurePoint(CPtPare Pt);
	CMeasurePoint():head(0),numPoint(0),testValue(0),measureWay(0){}
	CMeasurePoint(const CMeasurePoint& measurePt);//深度拷贝
	~CMeasurePoint();

	void addPoint(CPtPare x,bool increaseNum = true);
	//	void deletePoint();
	//	void setImgPt(ImgPOINT imgPt,int num = -1);
	//	void setGeoPt(GeoPOINT geoPt,int num = -1);
	void printMesure();
	void writeMesure(char *filePath);
	void readMesure(std::fstream& fp);

	void measure();
	void measureP2P();
	void measureP2L();
	void measureP2S();
	void measureAngle();
	void measureArea();
	void measureCurPos(const CGeoPOINT leftPos , const CGeoPOINT rightPos , const CGeoPOINT dotPos);

	CPtPare getPtPare(int num);
	CGeoPOINT getGeoPoint(int num);
	void clean();

	void operator=(const CMeasurePoint& measurePt);//深度拷贝
public:
	string ValueStr;
	double  current_pos;
};
