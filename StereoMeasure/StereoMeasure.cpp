#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "io.h"
#include <comdef.h>
#include "GeoPoint.h"
#include "ImgPoint.h"
#include "PtPare.h"
#include "CameralMat.h"
#include "MesurePoint.h"
#include "cv.h"

using namespace std;

//输入两幅图像上的同名点，三维重建
void reconstruct3d(
    const CImgPOINT& imgpnt_l,
    const CImgPOINT& imgpnt_r,
    const CCameralMat& mat,
    CGeoPOINT& pos)
{
    //1.以左相机为原点计算出左右相机的投影矩阵M1、M2
    //2.使用最小二乘法求出齐次方程的xyz（参见第五章立体视觉P73）

    //以左相机为基准坐标 世界坐标
	double tempL[]={1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f};

	//由R T计算右相机的坐标
	double tempR[3][4];
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			tempR[i][j]=cvmGet(mat.R, i, j);
		}
	}

	for(int i=0; i<3; i++)
	{
		tempR[i][3]=cvmGet(mat.T, i, 0);
	} 

    //左、右图像的R、T 组成3*4矩阵
    CvMat pRTL = cvMat(3, 4, CV_64F, tempL);
	CvMat pRTR = cvMat(3, 4, CV_64F, tempR);

    //矩阵乘法运算得到M1 3-3*3-4=3-4
	CvMat *pKMulRT1 = cvCreateMat(3, 4, CV_64F);
	cvGEMM(mat.M1, &pRTL, 1, NULL, 0, pKMulRT1, 0);
    //M2
	CvMat *pKMulRT2 = cvCreateMat(3, 4, CV_64F);
	cvGEMM(mat.M2, &pRTR, 1, NULL, 0, pKMulRT2, 0);

/*
    //the R
	double R31=(double)cvmGet(&pRTR,2,0);
	double R32=(double)cvmGet(&pRTR,2,1);
	double R33=(double)cvmGet(&pRTR,2,2);

    //
	double t1=(double)cvmGet(&pRTR,0,3);
	double t2=(double)cvmGet(&pRTR,1,3);
	double t3=(double)cvmGet(&pRTR,2,3);
*/

    //M1
	double m111=(double)cvmGet(pKMulRT1,0,0);
	double m112=(double)cvmGet(pKMulRT1,0,1);
	double m113=(double)cvmGet(pKMulRT1,0,2);
	double m114=(double)cvmGet(pKMulRT1,0,3);//

	double m121=(double)cvmGet(pKMulRT1,1,0);
	double m122=(double)cvmGet(pKMulRT1,1,1);
	double m123=(double)cvmGet(pKMulRT1,1,2);
	double m124=(double)cvmGet(pKMulRT1,1,3);
	//--	
	double m131=(double)cvmGet(pKMulRT1,2,0);
	double m132=(double)cvmGet(pKMulRT1,2,1);
	double m133=(double)cvmGet(pKMulRT1,2,2);
	double m134=(double)cvmGet(pKMulRT1,2,3);
	//--	

	//M2
	double m211=(double)cvmGet(pKMulRT2,0,0);
	double m212=(double)cvmGet(pKMulRT2,0,1);
	double m213=(double)cvmGet(pKMulRT2,0,2);
	double m214=(double)cvmGet(pKMulRT2,0,3);

	double m221=(double)cvmGet(pKMulRT2,1,0);
	double m222=(double)cvmGet(pKMulRT2,1,1);
	double m223=(double)cvmGet(pKMulRT2,1,2);
	double m224=(double)cvmGet(pKMulRT2,1,3);

	double m231=(double)cvmGet(pKMulRT2,2,0);
	double m232=(double)cvmGet(pKMulRT2,2,1);
	double m233=(double)cvmGet(pKMulRT2,2,2);
	double m234=(double)cvmGet(pKMulRT2,2,3);

    // 最小二乘法的输入矩阵
	CvMat *pAa=cvCreateMat(4,3,CV_64F);
    // 最小二乘法的线性系统右部
	CvMat *pBb=cvCreateMat(4,1,CV_64F);

	//fill A
	cvmSet(pAa, 0, 0, m111);
	cvmSet(pAa, 0, 1, m112);
	cvmSet(pAa, 0, 2, m113 - imgpnt_l.x);

	cvmSet(pAa, 1, 0, m121);
	cvmSet(pAa, 1, 1, m122);
	cvmSet(pAa, 1, 2, m123 - imgpnt_l.y);

	cvmSet(pAa, 2, 0, imgpnt_r.x*m231 - m211);
	cvmSet(pAa, 2, 1, imgpnt_r.x*m232 - m212);
	cvmSet(pAa, 2, 2, imgpnt_r.x*m233 - m213);

	cvmSet(pAa, 3, 0, imgpnt_r.y*m231 - m221);
	cvmSet(pAa, 3, 1, imgpnt_r.y*m232 - m222);
	cvmSet(pAa, 3, 2, imgpnt_r.y*m233 - m223);

	//fill B
	pBb->data.db[0]=-m114;
	pBb->data.db[1]=-m124;
	pBb->data.db[2]=m214 - imgpnt_r.x*m234;
	pBb->data.db[3]=m224 - imgpnt_r.y*m234;

	CvMat *A_T = cvCreateMat(3,4,CV_64F);
	CvMat *ATA = cvCreateMat(3,3,CV_64F);
	cvTranspose(pAa, A_T);//A_T是pAa的转置;
	cvMatMul(A_T, pAa, ATA);
	cvInvert(ATA, ATA);
	cvMatMul(ATA, A_T, A_T);

    //最小二乘法的输出矩阵pDisXYZreal,世界坐标
	CvMat *pDisXYZreal = cvCreateMat(3,1,CV_64F);
	cvMatMul(A_T, pBb, pDisXYZreal);
	pos.x = pDisXYZreal->data.db[0];
	pos.y = pDisXYZreal->data.db[1];
	pos.z = pDisXYZreal->data.db[2];

	cvReleaseMat(&pKMulRT1);
	cvReleaseMat(&pKMulRT2);
	cvReleaseMat(&pAa);
	cvReleaseMat(&pBb);
	cvReleaseMat(&pDisXYZreal);
}




extern "C" 
{

BOOL LoadCalibrationFile(BSTR FileName)
{
    char * strfilename=_com_util::ConvertBSTRToString(FileName);
	if (g_CameraMat.read(strfilename))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool __declspec(dllexport) __stdcall LoadCaliFile(char* filename)
{
	return g_CameraMat.read(filename);
}

bool __declspec(dllexport) __stdcall RecalifyImages(const char* ori_left,
    const char* ori_right, const char* cal_left, const char* cal_right)
{
    return g_CameraMat.RecalifyImages(ori_left, ori_right, cal_left, cal_right);
}

bool __declspec(dllexport) __stdcall UndistortedImage(const char* ori,
    const char* undistorted, bool isleft)
{
    return g_CameraMat.UndistortFoto(ori, undistorted, isleft);
}

double __declspec(dllexport) __stdcall MeasurePosition(int leftX,int leftY,int rightX,int rightY)
{
	double dValue=0.0;

    CMeasurePoint measurepoint;

	CImgPOINT left,right;
    CGeoPOINT DotPos;

	CGeoPOINT LeftCamera(0,0,0);
	CGeoPOINT RightCamera;
	RightCamera.x = cvmGet(g_CameraMat.T,0,0);
	RightCamera.y = cvmGet(g_CameraMat.T,1,0);
	RightCamera.z = cvmGet(g_CameraMat.T,2,0);

	left.x  = leftX;
	left.y  = leftY;
	right.x = rightX;
	right.y = rightY;
    reconstruct3d(left, right, g_CameraMat, DotPos);

	measurepoint.measureCurPos(LeftCamera , RightCamera ,DotPos);

	dValue = measurepoint.current_pos;

	return dValue;
}

double __declspec(dllexport) __stdcall MeasureDistance1(int* leftPt1,
    int* rightPt1, int* leftPt2, int* rightPt2)
{
	double dValue=0.0;

    CMeasurePoint measurepoint;

	CImgPOINT left,right;
	CGeoPOINT DotPos;



	left.x =leftPt1[0];
	left.y = leftPt1[1];
	right.x = rightPt1[0];
	right.y =  rightPt1[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	left.x =leftPt2[0];
	left.y = leftPt2[1];
	right.x = rightPt2[0];
	right.y =  rightPt2[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	measurepoint.measureWay=2;
	measurepoint.measure();

    dValue=measurepoint.testValue;
	return dValue;
}


double __declspec(dllexport) __stdcall MeasureDistance(
    long left_x1,
    long left_y1,
    long left_x2,
    long left_y2,
    long right_x1,
    long right_y1,
    long right_x2,
    long right_y2)
{
    int left1[2];
    int left2[2];
    int right1[2];
    int right2[2];

    left1[0] = left_x1;
    left1[1] = left_y1;
    left2[0] = left_x2;
    left2[1] = left_y2;
    right1[0] = right_x1;
    right1[1] = right_y1;
    right2[0] = right_x2;
    right2[1] = right_y2;

    return MeasureDistance1(left1, right1, left2, right2);
}


double __declspec(dllexport) __stdcall MeasureAreaTriangle1(int* leftPt1,int* rightPt1,int* leftPt2,int* rightPt2,int* leftPt3,int* rightPt3)
{
	double dValue=0.0;

	CMeasurePoint measurepoint;

	CImgPOINT left,right;
	CGeoPOINT DotPos;

	left.x =leftPt1[0];
	left.y = leftPt1[1];
	right.x = rightPt1[0];
	right.y =  rightPt1[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	left.x =leftPt2[0];
	left.y = leftPt2[1];
	right.x = rightPt2[0];
	right.y =  rightPt2[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	left.x =leftPt3[0];
	left.y = leftPt3[1];
	right.x = rightPt3[0];
	right.y =  rightPt3[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	measurepoint.measureWay=3;
	measurepoint.measure();

	dValue=measurepoint.testValue;

	return dValue;
}

double __declspec(dllexport) __stdcall MeasureAreaTriangle(
    long left_x1,
    long left_y1,
    long left_x2,
    long left_y2,
    long left_x3,
    long left_y3,
    long right_x1,
    long right_y1,
    long right_x2,
    long right_y2,
    long right_x3,
    long right_y3)
{
    int left1[2];
    int left2[2];
    int left3[2];
    int right1[2];
    int right2[2];
    int right3[2];

    left1[0] = left_x1;
    left1[1] = left_y1;
    left2[0] = left_x2;
    left2[1] = left_y2;
    left3[0] = left_x3;
    left3[1] = left_y3;
    right1[0] = right_x1;
    right1[1] = right_y1;
    right2[0] = right_x2;
    right2[1] = right_y2;
    right3[0] = right_x3;
    right3[1] = right_y3;

    return MeasureAreaTriangle1(left1, right1, left2, right2, left3, right3);
}

double __declspec(dllexport) __stdcall MeasureAngle(int* leftPt1,int* rightPt1,int* leftPt2,int* rightPt2,int* leftPt3,int* rightPt3)
{
	double dValue=0.0;
	CMeasurePoint measurepoint;

	CImgPOINT left,right;
	CGeoPOINT DotPos;

	left.x =leftPt1[0];
	left.y = leftPt1[1];
	right.x = rightPt1[0];
	right.y =  rightPt1[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	left.x =leftPt2[0];
	left.y = leftPt2[1];
	right.x = rightPt2[0];
	right.y =  rightPt2[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	left.x =leftPt3[0];
	left.y = leftPt3[1];
	right.x = rightPt3[0];
	right.y =  rightPt3[1];
	reconstruct3d(left, right, g_CameraMat, DotPos);
	measurepoint.addPoint(CPtPare(left,right,DotPos));

	measurepoint.measureWay=5;
	measurepoint.measure();

	dValue=measurepoint.testValue;

	return dValue;
}

}