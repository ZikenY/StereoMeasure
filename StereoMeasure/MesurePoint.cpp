#include "StdAfx.h"
#include "MesurePoint.h"
#include <math.h>



double length(const CGeoPOINT& pt)
{
	return sqrt(pow(pt.x,2)+pow(pt.y,2)+pow(pt.z,2));
}

double triangleArea(const CGeoPOINT& pt1,const CGeoPOINT& pt2)
{
	double length1 = length(pt1);
	double length2 = length(pt2);
	double cos = (pt1.x*pt2.x + pt1.y*pt2.y + pt1.z*pt2.z)/(length1*length2);
	return length1*length2*sqrt(1-cos*cos)/2;
}

bool operator==(const CGeoPOINT &lpt,const CGeoPOINT &rpt)
{
	return (lpt.x == rpt.x) && (lpt.y == rpt.y) && (lpt.z == rpt.z);
}

bool operator!=(const CGeoPOINT &lpt,const CGeoPOINT &rpt)
{
	return !(lpt == rpt);
}

CGeoPOINT operator- (const CGeoPOINT &lpt,const CGeoPOINT &rpt)
{
	CGeoPOINT temp(lpt);
	temp -= rpt;
	return temp;
}

CGeoPOINT operator* (const CGeoPOINT &p1,const CGeoPOINT &p2)
{
	CGeoPOINT temp;
	temp.x = p1.y * p2.z - p1.z * p2.y;
	temp.y = p1.z * p2.x - p1.x * p2.z;
	temp.z = p1.x * p2.y - p1.y * p2.x;
	return temp;
}

ostream& operator<<(ostream& out,const CGeoPOINT &pt)
{
	out/*<<"( x , y , z )   "*/
		<<"("<<pt.x<<","
		<<pt.y<<","
		<<pt.z<<")";
	return out;
}




CMeasurePoint::CMeasurePoint(CPtPare Pt)
: ValueStr("")
{
	head = new CPtPare(Pt);
	numPoint = 1;
}
CMeasurePoint::CMeasurePoint(const CMeasurePoint& measurePt)
{
	head = new CPtPare(*(measurePt.head));
	measureWay = measurePt.measureWay;
	numPoint = measurePt.numPoint;
	testValue = measurePt.testValue;

	CPtPare* measureNext = measurePt.head->next;
	CPtPare* pNext = head;
	while(measureNext)
	{
		CPtPare* temp = new CPtPare(*measureNext);
		pNext->next = temp;
		measureNext = measureNext->next;
		pNext = pNext->next;
	}
}

void CMeasurePoint::operator=(const CMeasurePoint& measurePt)
{
	head = new CPtPare(*(measurePt.head));
	measureWay = measurePt.measureWay;
	numPoint = measurePt.numPoint;
	testValue = measurePt.testValue;

	CPtPare* measureNext = measurePt.head->next;
	CPtPare* pNext = head;
	while(measureNext)
	{
		CPtPare* temp = new CPtPare(*measureNext);
		pNext->next = temp;
		measureNext = measureNext->next;
		pNext = pNext->next;
	}	
}

void CMeasurePoint::addPoint(CPtPare x,bool increaseNUM)
{
	CPtPare* temp = new CPtPare(x);

	if(head == 0)
	{
		head = temp;
	}

	else
	{
		CPtPare *p = head;
		while(p->next != NULL)
		{
			p = p->next;
		}

		p->next = temp;
	}
	if(increaseNUM)
		numPoint ++;
}



CMeasurePoint::~CMeasurePoint()
{
	CPtPare *p = head;
	while(p)
	{
		CPtPare *temp = p->next;
		delete p;
		p = temp;
	}
}

void CMeasurePoint::clean()
{
	CPtPare *p = head;
	while(p)
	{
		CPtPare *temp = p->next;
		delete p;
		p = temp;
	}
	head = 0;
	numPoint = 0;
	testValue = 0;
	measureWay = 0;
}

void CMeasurePoint::readMesure(std::fstream& fp)
{
	// 	CPtPare *p = head;
	// 	int pos = -1;
	// 	CString line;
	// 	CString data;
	// 	int dataLen;
	// 	CPtPare temp;
	// 
	// 	char a[1024];
	// 	for (int i=0;i<1024;i++)
	// 		a[i]='0';
	// 
	// 	while(pos == -1&& !fp.eof())
	// 	{
	// 	fp.getline(a,256);
	// 	line=CString(a);
	// 	pos = line.Find(':');
	// 	}
	// 	dataLen = line.GetLength()-pos-1;
	// 	data = line.Right(dataLen);
	// 	measureWay = atoi(data.GetBuffer(0));
	// 	pos = -1;
	// 
	// 	while(pos == -1&& !fp.eof())
	// 	{
	// 	fp.getline(a,1024);
	// 	line=CString(a);
	// 	pos = line.Find(':');
	// 	}
	// 	dataLen = line.GetLength()-pos-1;
	// 	data = line.Right(dataLen);
	// 	testValue =(float)atof(data.GetBuffer(0));
	// 	pos = -1;
	// 
	// 	while(pos == -1&& !fp.eof())
	// 	{
	// 	fp.getline(a,1024);
	// 	line=CString(a);
	// 	pos = line.Find(':');
	// 	}
	// 	dataLen = line.GetLength()-pos-1;
	// 	data = line.Right(dataLen);
	// 	numPoint=atoi(data.GetBuffer(0));
	// 	pos = -1;
	// 
	// 	if(fp.eof())
	// 		return;
	// 
	// 
	// 	for(int j=0;j<numPoint;j++)
	// 	{
	// 		fp.getline(a,1024);
	// 		line=CString(a);
	// 
	// 		int posGeoBegin = line.Find('(');
	// 		int posGeoEnd = line.Find(')');
	// 		data = line.Mid(posGeoBegin+1,posGeoEnd);
	// 		int firstGeo = data.Find(',');
	// 		temp.geoPt.x = (float)atof(data.Left(firstGeo).GetBuffer(0));
	// 
	// 		int lastGeo = data.ReverseFind(',');
	// 		temp.geoPt.y = (float)atof(data.Mid(firstGeo+1,lastGeo));
	// 		temp.geoPt.z = (float)atof(data.Right(data.GetLength()-lastGeo-1).GetBuffer(0));
	// 		
	// 		int posImg_RBegin = line.ReverseFind('(');
	// 		int posImg_REnd = line.ReverseFind(')');
	// 
	// 		int posImg_LBegin = posGeoEnd+2;
	// 		int posImg_LEnd = posImg_RBegin-2;
	// 
	// 		data = line.Mid(posImg_LBegin+1,posImg_LEnd);
	// 		int firstImg_L = data.Find(',');
	// 		temp.imgPt_L.x = atoi(data.Left(firstImg_L).GetBuffer(0));
	// 		temp.imgPt_L.y = atoi(data.Right(data.GetLength()-firstImg_L-1));
	// 
	// 		data = line.Mid(posImg_RBegin+1,posImg_REnd);
	// 		int firstImg_R = data.Find(',');
	// 		temp.imgPt_R.x = atoi(data.Left(firstImg_R).GetBuffer(0));
	// 		temp.imgPt_R.y = atoi(data.Right(data.GetLength()-firstImg_R-1));
	// 		
	// 		addPoint(temp,false);
	// 	}
}

void CMeasurePoint::writeMesure(char* filePath)
{
	//std::locale::global(locale(""));
	//fstream fs;
	//fs.open(filePath,ofstream::out | ofstream::app |ofstream::end);
	//CPtPare *p = head;

	//fs<<"\nmeasure way:"<<measureWay<<endl;
	//fs<<"Mesure value:"<<testValue<<endl;
	//fs<<"num of point: "<<numPoint<<endl;
	//while(p)
	//{
	//	fs<<p->geoPt<<" "<<p->imgPt_L<<" "<<p->imgPt_R<<endl;
	//	p = p->next;
	//}
	//fs<<"---------------------------------------"<<endl;
	//fs.close();
	//locale::global(locale("C"));
}

void CMeasurePoint::printMesure()
{
	CPtPare *p = head;
	//cout<<"num of point: "<<numPoint<<endl;
	//cout<<"measure way:"<<measureWay<<endl;
	//cout<<"Mesure value:"<<testValue<<endl;
	//while(p)
	//{
	//	cout<<p->imgPt_L<<endl;
	//	cout<<p->imgPt_R<<endl;
	//	cout<<p->geoPt<<endl;
	//	cout<<"-------------------"<<endl;
	//	p = p->next;
	//}
}

CPtPare CMeasurePoint::getPtPare(int num)
{
	int i=1;
	CPtPare *p = head;

	while(i != num)
	{
		p = p->next;
		i++;
	}

	return *p;
}

CGeoPOINT CMeasurePoint::getGeoPoint(int num)
{
	int i=1;
	CPtPare *p = head;

	while(i != num)
	{
		p = p->next;
		i++;
	}

	return p->geoPt;
}



void CMeasurePoint::measure()
{
	switch (measureWay)
	{
	case 2:
		measureP2P();
		break;
	case 3:
		measureArea();
		break;
	case 4:
		measureP2L();
		break;
	case 5:
		measureAngle();
		break;
	case 6:
		measureP2S();
		break;
	default:
		break;
	}
}
void CMeasurePoint::measureAngle()
{
	if( numPoint !=3)
		return;

	CGeoPOINT p0 = getGeoPoint(1);
	CGeoPOINT L1 = getGeoPoint(2);
	CGeoPOINT L2 = getGeoPoint(3);

	L2 -= L1;
	p0 -= L1;

	double length1 = length(p0);
	double length2 = length(L2);
	double angle = acos((p0.x*L2.x + p0.y*L2.y + p0.z*L2.z)/(length1*length2));//点乘除以长度为cos值
	testValue = angle*180/3.1415926f;

	//ValueStr.Format(_T("%7.4f 度"), testValue);

}

void CMeasurePoint::measureArea()
{
	if(numPoint!=3)
		return;

	CGeoPOINT head = getGeoPoint(1);//多边形的起始顶点
	CGeoPOINT triangle1;//三角形另外顶点1
	CGeoPOINT triangle2;//三角形另外顶点1
	double Area = 0;
	triangle1 = getGeoPoint(2);
	triangle2 = getGeoPoint(3);
	triangle1 -= head;//边向量1
	triangle2 -= head;//边向量2
	Area= triangleArea(triangle1,triangle2);
	testValue = Area;


	//CString str;
	//if(testValue>=10000)
	//{
	//	ValueStr.Format(_T("%7.4f m*m"),testValue/10000);
	//}
	//else
	//{
	//	ValueStr.Format(_T("%7.4f cm*cm"),testValue);
	//}
}

void CMeasurePoint::measureP2S()
{
	if(numPoint !=4)
		return;

	CGeoPOINT p0 = getGeoPoint(1);
	CGeoPOINT s1 = getGeoPoint(2);
	CGeoPOINT s2 = getGeoPoint(3);
	CGeoPOINT s3 = getGeoPoint(4);
	CGeoPOINT s4;

	s4 = s2 - s1;
	s3 -=s1;
	s4 = s4 * s3;
	p0 -=s1;

	//(A,B,C)为法向量，值为面上两向量的叉乘
	//d = |A*(x0-x1) + B*(y0-y1) + C*(z0-z1)|/(A*A + B*B + C*C)^1/2
	testValue = sqrt(pow(s4.x*p0.x+s4.y*p0.y+s4.z*p0.z,2)/(pow(s4.x,2)+pow(s4.y,2)+pow(s4.z,2)));
	//if (testValue>100)
	//{
	//	ValueStr.Format(_T("%7.4f m"),testValue/100);
	//} 
	//else
	//{
	//	ValueStr.Format(_T("%7.4f cm"),testValue);
	//}
}

void CMeasurePoint::measureP2P()
{

	if(numPoint !=2)
		return;

	CGeoPOINT pt1 = getGeoPoint(1);
	CGeoPOINT pt2 = getGeoPoint(2);
	pt1 -= pt2;

	testValue = length(pt1);
	//if (testValue>100)
	//{
	//	ValueStr.Format(_T("%7.4f m"),testValue/100);
	//} 
	//else
	//{
	//	ValueStr.Format(_T("%7.4f cm"),testValue);
	//}

}

void CMeasurePoint::measureP2L()
{
	if(numPoint !=3)
		return;

	CGeoPOINT p0 = getGeoPoint(1);
	CGeoPOINT L1 = getGeoPoint(2);
	CGeoPOINT L2 = getGeoPoint(3);

	L2 -= L1;
	p0 -= L1;

	double length1 = length(p0);
	double length2 = length(L2);
	double cos = (p0.x*L2.x + p0.y*L2.y + p0.z*L2.z)/(length1*length2);
	testValue = length1*sqrt(1-cos*cos);

	//if (testValue>100)
	//{
	//	ValueStr.Format(_T("%7.4f m"),testValue/100);
	//} 
	//else
	//{
	//	ValueStr.Format(_T("%7.4f cm"),testValue);
	//}

}

void CMeasurePoint::measureCurPos(const CGeoPOINT leftPos , const CGeoPOINT rightPos , const CGeoPOINT dotPos)
{
	CGeoPOINT p0 =  dotPos;
	CGeoPOINT L1 =  leftPos;
	CGeoPOINT L2 =  rightPos;

	L2 -= L1;
	p0 -= L1;

	double length1 = length(p0);
	double length2 = length(L2);
	double cos = (p0.x*L2.x + p0.y*L2.y + p0.z*L2.z)/(length1*length2);
	current_pos = length1*sqrt(1-cos*cos);

}