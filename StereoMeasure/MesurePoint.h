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
	int numPoint;//��������
	double  testValue;//����ֵ
	int measureWay;//������ʽ��0����ǣ�1���㵽�㣻2���㵽�ߣ�3���㵽�棻4���Ƕȣ�5�����

public:
	CMeasurePoint(CPtPare Pt);
	CMeasurePoint():head(0),numPoint(0),testValue(0),measureWay(0){}
	CMeasurePoint(const CMeasurePoint& measurePt);//��ȿ���
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

	void operator=(const CMeasurePoint& measurePt);//��ȿ���
public:
	string ValueStr;
	double  current_pos;
};
