#include "stdafx.h"
#include "cameralMat.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "cxmisc.h"
#include "highgui.h"
#include <ctype.h>

using namespace std;

ostream& operator<<(ostream& out,const CvMat &pt)
{
	int i,j;
	int clos = pt.cols;
	double *data=(double*)pt.data.db;

	for(j=0;j<pt.rows;j++)
		for(i=0;i<clos;i++)
			out<<data[j*clos+i]<<" ";

	out<<endl;
	return out;
}

istream& operator>>(istream& in,const CvMat &pt)
{
	int i,j;
	int clos = pt.cols;
	double *data=(double*)pt.data.db;

	for(j=0;j<pt.rows;j++)
		for(i=0;i<clos;i++)
			in>>data[j*clos+i];

	return in;
}

void CCameralMat::init()
{
    mx1 = NULL;
    my1 = NULL;
    mx2 = NULL;
    my2 = NULL;
    bmstate = NULL;
    disp = NULL;
    this->ReleaseMaps();
}

CCameralMat::CCameralMat()
{
    this->init();

    M1 = cvCreateMat(3, 3, CV_64F);
	M2 = cvCreateMat(3, 3, CV_64F);
	D1 = cvCreateMat(1, 5, CV_64F);
	D2= cvCreateMat(1, 5, CV_64F);
	R = cvCreateMat(3, 3, CV_64F);
	T = cvCreateMat(3, 1, CV_64F);
	E = cvCreateMat(3, 3, CV_64F);
	F = cvCreateMat(3, 3, CV_64F); 
	avgErr = 0;
}

CCameralMat::CCameralMat(const CCameralMat& mat)
{
    this->init();

    cvCopy(mat.M1,M1);
	cvCopy(mat.M2,M2);
	cvCopy(mat.D1,D1);
	cvCopy(mat.D2,D2);
	cvCopy(mat.R,R);
	cvCopy(mat.T,T);
	cvCopy(mat.E,E);
	cvCopy(mat.F,F);
	avgErr = mat.avgErr;
}

void CCameralMat::operator =(CCameralMat& mat)
{  
    this->init();

    cvCopy(mat.M1,M1);
	cvCopy(mat.M2,M2);
	cvCopy(mat.D1,D1);
	cvCopy(mat.D2,D2);
	cvCopy(mat.R,R);
	cvCopy(mat.T,T);
	cvCopy(mat.E,E);
	cvCopy(mat.F,F);
	avgErr = mat.avgErr;
}

CCameralMat::~CCameralMat()
{
    this->ReleaseMaps();

    cvReleaseMat(&M1);
	cvReleaseMat(&M2);
	cvReleaseMat(&D1);
	cvReleaseMat(&D2);
	cvReleaseMat(&R);
	cvReleaseMat(&T);
	cvReleaseMat(&E);
	cvReleaseMat(&F);
}

void CCameralMat::save(const char *filePath)
{
	std::locale::global(locale(""));
	fstream fs;
	fs.open(filePath,ofstream::out | ofstream::app | ofstream::end);

	fs<<"S"<<endl;
	fs<<*M1;
	fs<<*M2;
	fs<<*D1;
	fs<<*D2;
	fs<<*R;
	fs<<*T;
	fs<<*E;
	fs<<*F;
	fs<<avgErr<<endl;

	fs<<"END"<<endl;
	fs.close();
	locale::global(locale("C"));
}


bool CCameralMat::read(const char *filePath)
{
    this->ReleaseMaps();
    std::locale::global(locale(""));
	fstream fp(filePath,ios_base::in);
	int pos = -1;
	string line;

	char a[1024];
	memset(a, 0, 1024);
   
    if(!fp)
    {
		return false;
    }

	while(pos == -1&& !fp.eof())
	{
		fp.getline(a,256);
		line=a;
		pos = line.find('S');
	}

	fp>>*M1;
	fp>>*M2;
	fp>>*D1;
	fp>>*D2;
	fp>>*R;
	fp>>*T;
	fp>>*E;
	fp>>*F;
	fp>>avgErr;

	fp.close();
    return true;
}

void CCameralMat::ReleaseMaps()
{
    if (mx1)
    {
        cvReleaseMat(&mx1);
        cvReleaseMat(&my1);
        cvReleaseMat(&mx2);
        cvReleaseMat(&my2);
    }

    mx1 = NULL;
    my1 = NULL;
    mx2 = NULL;
    my2 = NULL;
    imageSize.height = 0;
    imageSize.width = 0;

    if (bmstate)
        cvReleaseStereoBMState(&bmstate);
    if (disp)
        cvReleaseMat(&disp);
    bmstate = NULL;
    disp = NULL;
}

void CCameralMat::CalcRecalifiedMaps(const CvSize& newsize)
{
    if ((mx1) && (imageSize.height == newsize.height) && (imageSize.width == newsize.width))
        return; //不需了

    //don't forget
    this->ReleaseMaps();
    this->imageSize = newsize;

    double R1[3][3], R2[3][3], P1[3][4], P2[3][4];
    CvMat _R1 = cvMat(3, 3, CV_64F, R1);
    CvMat _R2 = cvMat(3, 3, CV_64F, R2);
    CvMat _P1 = cvMat(3, 4, CV_64F, P1);
    CvMat _P2 = cvMat(3, 4, CV_64F, P2);

    // IF BY CALIBRATED (BOUGUET'S METHOD)
    cvStereoRectify(
        M1, M2, D1, D2, imageSize,
        R, T,
        &_R1, &_R2, &_P1, &_P2, 0
//        , CV_CALIB_ZERO_DISPARITY
        );

    mx1 = cvCreateMat(imageSize.height, imageSize.width, CV_32F);
    my1 = cvCreateMat(imageSize.height, imageSize.width, CV_32F);
    mx2 = cvCreateMat(imageSize.height, imageSize.width, CV_32F);
    my2 = cvCreateMat(imageSize.height, imageSize.width, CV_32F);

    //Precompute maps for cvRemap()
    cvInitUndistortRectifyMap(M1, D1, &_R1, &_P1, mx1, my1);
    cvInitUndistortRectifyMap(M2, D2, &_R2, &_P2, mx2, my2);

    //Setup for finding stereo corrrespondences
    bmstate = cvCreateStereoBMState();
    assert(bmstate != 0);
    bmstate->preFilterSize=41;
    bmstate->preFilterCap=31;
    bmstate->SADWindowSize=41;
    bmstate->minDisparity=-64;
    bmstate->numberOfDisparities=128;
    bmstate->textureThreshold=10;
    bmstate->uniquenessRatio=15;
    disp = cvCreateMat(imageSize.height, imageSize.width, CV_16S);
}

bool CCameralMat::RecalifyImages(const char* ori_left, const char* ori_right,
        const char* cal_left, const char* cal_right)
{
    IplImage* img1 = cvLoadImage(ori_left, CV_LOAD_IMAGE_COLOR);
    IplImage* img2 = cvLoadImage(ori_right, CV_LOAD_IMAGE_COLOR);

    if (!img1 || !&img2)
    {
        return false;
    }

    CvSize imageSize = cvGetSize(img1);

    this->CalcRecalifiedMaps(imageSize);

    IplImage* img1r = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
    IplImage* img2r = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
    cvRemap(img1, img1r, mx1, my1);
    cvRemap(img2, img2r, mx2, my2);

  // When the stereo camera is oriented vertically,
  // useUncalibrated==0 does not transpose the
  // image, so the epipolar lines in the rectified
  // images are vertical. Stereo correspondence
  // function does not support such a case.
//    cvFindStereoCorrespondenceBM(img1r, img2r, disp, bmstate);

    cvSaveImage(cal_left, img1r);
    cvSaveImage(cal_right, img2r);

    cvReleaseImage(&img1r);
    cvReleaseImage(&img2r);
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);

    return true;
}



bool CCameralMat::UndistortFoto(const char* ori, const char* undistorted, bool isleft)
{
    IplImage* foto1 = cvLoadImage(ori);
    if (!foto1)
        return false;

    CvSize size = cvGetSize(foto1);
    CvMat* intrinsics = NULL;
    CvMat* distortion_coeff = NULL;
    if (isleft)
    {
        intrinsics = M1;
        distortion_coeff = D1;
    }
    else
    {
        intrinsics = M2;
        distortion_coeff = D2;
    }

    //畸变映射矩阵(两个单独的32位单通道矩阵，分别表示x坐标下的映射和y坐标下的映射)
    IplImage *mapx = cvCreateImage(size, IPL_DEPTH_32F, 1);
    IplImage *mapy = cvCreateImage(size, IPL_DEPTH_32F, 1);

    //计算形变与非形变图像的映射(前两个参数为标定得到的摄像机内参数矩阵和畸变系数)
    cvInitUndistortMap(intrinsics, distortion_coeff, mapx, mapy);

    IplImage* foto2 = cvCreateImage(size, 8, 3);

    //对图像进行普通几何变换：利用指定的映射(即mapx和mapy)将源图像转换到目标图像中
    cvRemap(foto1, foto2, mapx, mapy);

//    string dir = GetDirectoryPart(undistorted);
//    ::CreateDirectory(dir.c_str(), NULL);
    cvSaveImage(undistorted, foto2);
//    cvNamedWindow("xx",0);
//    cvShowImage("xx",foto2);

    cvReleaseImage(&foto1);
    cvReleaseImage(&foto2);
    cvReleaseImage(&mapx);
    cvReleaseImage(&mapy);

    return true;
}

