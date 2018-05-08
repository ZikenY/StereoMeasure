#pragma warning (disable:4996)
#pragma warning (disable:4018)
#pragma warning (disable:4244)
#pragma warning (disable:4793)
#pragma warning (disable:4786)
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cv.h>
#include "highgui.h"
#include "cameralMat.h"
#include "..\\wplib\\StringFuncs.h"

using namespace std;
using namespace weipan;
#define NULL    0

typedef bool (_stdcall progresscallback)(
    const long progress,
    const char* msg,
    const char* fotofilename,
    bool isleft);

//pictures必须是一左一右
string DoStereoCalib(const vector<string>& pictures, int chessboard_width,
    int chessboard_height, double squaresize, double ccdpixelsize, CCameralMat& mat,
    progresscallback* pProgressHandle)
{
    string errmsg;

	CvMat *M1 = mat.M1; //左相机内参数矩阵
	CvMat *M2 = mat.M2; //右
	CvMat *D1 = mat.D1; //左相机畸变参数
	CvMat *D2 = mat.D2; //右
	CvMat *R  = mat.R;  //旋转矩阵
	CvMat *T  = mat.T;  //平移向量
	CvMat *E  = mat.E;  //本征矩阵
	CvMat *F  = mat.F;  //基础矩阵 

	vector<CvPoint3D32f>    objectPoints;	        //棋盘角点的世界坐标系坐标（z=0）
	vector<CvPoint2D32f>    lr_corner_points[2];    //左右角点坐标列表（0-left；1-right）
	vector<int>             cors_on_valid_pic;      //每张有效图像的有效角点数
	CvSize                  imagesize = {0,0};      //图片大小
	vector<int>             identified_cor_pnt_counts[2];  //成功找到的角点数（这里01是多此一举，左右肯定要一致）
	int                     i = 0,
                            j = 0,
                            cor_count = chessboard_width*chessboard_height,   //单张图片的角点数
                            is_right = 0,           //左图还是右
                            valid_pic_count = 0,    //有效图片数
                            allcount=0;             //定义总帧数和总角点数

	vector<CvPoint2D32f>    temp_cors(cor_count);   //临时存储角点坐标

	for (i=0; i<pictures.size(); i++)
    {
        //左还是右
        is_right = i % 2;
        //图像角点坐标
		vector<CvPoint2D32f>& pts = lr_corner_points[is_right];

		IplImage *img = cvLoadImage(pictures[i].c_str(), 0);
		if (!img)
        {
            if (pProgressHandle)
            {
                string errmsg = "打开图片文件错误：" + pictures[i];
                (*pProgressHandle)(0, errmsg.c_str(), NULL, false);
            }

			return "";
		}

		imagesize = cvGetSize(img);

        //每幅图识别出来的角点数，这里没有用上因为必须全部识别出cor_count个
		int corcount = 0;

		//检测角点数，若检测失败，则返回0；
        //同时将角点像素坐标存于temp[];角点数存于count;
		int result = cvFindChessboardCorners(
            img, 
			cvSize(chessboard_width, chessboard_height),
			&temp_cors[0],
			&corcount,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE
            );

		allcount = (int)pts.size();
        //扩展角点总数
		pts.resize(allcount + cor_count, cvPoint2D32f(0,0));

        string cornerfoto;
		if (0 == result)
        {
			if (0 == is_right)
            { //直接跳过相同序号的右边图像
				i++;
			}
			else
            {
				identified_cor_pnt_counts[0].pop_back(); //删除相同序号的左边图像
			}
		}
		else
        {
			//——————显示角点图片————————
			IplImage *cimg = cvCreateImage(imagesize,8,3);
			cvCvtColor(img,cimg,CV_GRAY2BGR);
			cvDrawChessboardCorners(cimg, cvSize(chessboard_width, chessboard_height), &temp_cors[0], corcount, result);
//			cvShowImage("Corner",cimg);
            char temppath[MAX_PATH];
            GetTempPath(MAX_PATH, temppath);
            cornerfoto = temppath;
            cornerfoto = cornerfoto + "\\~calibratecornershow.jpg";
            cvSaveImage(cornerfoto.c_str(), cimg);
			cvReleaseImage(&cimg);
			//——————显示角点图片————————

            identified_cor_pnt_counts[is_right].push_back(result);

			//检测亚像素角点
			cvFindCornerSubPix(
                img,
                &temp_cors[0],
                corcount,
                cvSize(11, 11),
                cvSize(-1,-1),
                cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30, 0.01)
                );

			copy(temp_cors.begin(), temp_cors.end(), pts.begin() + allcount);
		}	

		cvReleaseImage(&img);

		//设置进度条
        if (pProgressHandle)
        {
            if (!(*pProgressHandle)((float)i/(float)pictures.size()*100,
                "检测棋盘角点...", cornerfoto.c_str(), (0 == is_right)))
            {
                return "";
            }
        }
	}


	//检测到角点的图片帧数（单边）
    valid_pic_count = (int)identified_cor_pnt_counts[0].size();
	if (0 == valid_pic_count)
    {
        if (pProgressHandle)
        {
            (*pProgressHandle)(0, "未检测到任何角点，请检查标定板尺寸设置是否正确！", NULL, false);
        }

        return "";
	}

	//初始化棋盘角点的世界坐标系坐标，z为0
    //这里搞定第一张图
	objectPoints.resize(valid_pic_count * cor_count);
	for( i = 0; i < chessboard_height; i++ )
    {
		for( j = 0; j < chessboard_width; j++ )
        {
			objectPoints[i*chessboard_width + j] = cvPoint3D32f(i*squaresize, j*squaresize, 0);
		}
	}

	//依次搞定后面几张图的棋盘角点世界坐标，直接复制第一张图的
	for( i = 1; i < valid_pic_count; i++ )
    {
		copy(objectPoints.begin(),
             objectPoints.begin() + cor_count,
             objectPoints.begin() + i*cor_count);
	}

    //每张有效图片的有效角点数，都是废话，肯定是cor_count个嘛
	cors_on_valid_pic.resize(valid_pic_count, cor_count);

    //全部有效角点数（单边）
	allcount = valid_pic_count*cor_count;

    //定义像素坐标、世界坐标系坐标、焦点总数数组
	CvMat sc_objectPoints = cvMat(1, allcount, CV_32FC3, &objectPoints[0] );
	CvMat sc_imagePoints1 = cvMat(1, allcount, CV_32FC2, &lr_corner_points[0][0] );
	CvMat sc_imagePoints2 = cvMat(1, allcount, CV_32FC2, &lr_corner_points[1][0] );
	CvMat sc_npoints = cvMat(1, cors_on_valid_pic.size(), CV_32S, &cors_on_valid_pic[0] );
	cvSetIdentity(M1);
	cvSetIdentity(M2);
	cvZero(D1);
	cvZero(D2);

    if (pProgressHandle)
    {
        if (!(*pProgressHandle)(60, "cvStereoCalibrate()...", NULL, false))
        {
            return "";
        }
    }

	cvStereoCalibrate(
        &sc_objectPoints,   //棋盘角点物理坐标（z=0）
		&sc_imagePoints1,   //棋盘角点在左相机上的图像坐标
		&sc_imagePoints2,   //右相机，来自cvFindChessboardCorners()
        &sc_npoints,        //每幅棋盘的角点个数，其实都是cor_count个
		M1, D1, M2, D2,     //左右相机的内参数矩阵与畸变矩阵
		imagesize,          //图像大小（像素）
		R, T, E, F,         //输出R T E F矩阵
		cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5),
		CV_CALIB_FIX_ASPECT_RATIO + CV_CALIB_ZERO_TANGENT_DIST + CV_CALIB_SAME_FOCAL_LENGTH
        );
/*
	cvSave("M1.xml",M1);    //左相机内参数矩阵
	cvSave("M2.xml",M2);    //右
	cvSave("D1.xml",D1);    //左相机畸变参数
	cvSave("D2.xml",D2);    //右
	cvSave("R.xml",R);      //旋转矩阵
	cvSave("T.xml",T);      //平移向量
	cvSave("E.xml",E);      //本征矩阵
	cvSave("F.xml",F);      //基础矩阵
*/

    //标定结果报表：
    float pixel_size = (float)ccdpixelsize;
//    float pixel_size = (float)4.40e-3;
    Strings report;

    //left camera
    report.AppendLine("  -- left camera --");
    float report_val = cvmGet(M1,0,0)*pixel_size;
    string report_line = "lens focus: " + FloatToStr(report_val) + "mm";
    report.AppendLine(report_line); //焦距
    report_val = cvmGet(D1, 0, 0);
    report_line = "lens radial distortion k1: " + FloatToStr(report_val);
    report.AppendLine(report_line); //径向畸变k1, k2, k3
    report_val = cvmGet(D1, 0, 1);
    report_line = "lens radial distortion k2: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D1, 0, 4);
    report_line = "lens radial distortion k3: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D1, 0, 2);  //切向畸变p1，p2
    report_line = "lens tangential distortion p1: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D1, 0, 3);
    report_line = "lens tangential distortion p2: " + FloatToStr(report_val);
    report.AppendLine(report_line);

    //right camera
    report.AppendLine(" ");
    report.AppendLine("  -- right camera --");
    report_val = cvmGet(M2,0,0)*pixel_size;
    report_line = "lens focus: " + FloatToStr(report_val) + "mm";
    report.AppendLine(report_line);
    report_val = cvmGet(D2, 0, 0);
    report_line = "lens radial distortion k1: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D2, 0, 1);
    report_line = "lens radial distortion k2: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D2, 0, 4);
    report_line = "lens radial distortion k3: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D2, 0, 2);  //切向畸变p1，p2
    report_line = "lens tangential distortion p1: " + FloatToStr(report_val);
    report.AppendLine(report_line);
    report_val = cvmGet(D2, 0, 3);
    report_line = "lens tangential distortion p2: " + FloatToStr(report_val);
    report.AppendLine(report_line);


    if (pProgressHandle)
    {
        if (!(*pProgressHandle)(95, "计算误差", NULL, false))
        {
            return "";
        }
    }

	//极限约束
	vector<CvPoint3D32f> lines[2];
	lr_corner_points[0].resize(allcount);
	lr_corner_points[1].resize(allcount);
	lines[0].resize(allcount);
	lines[1].resize(allcount);
	CvMat cce_L1 = cvMat(1, allcount, CV_32FC3, &lines[0][0]);
	CvMat cce_L2 = cvMat(1, allcount, CV_32FC3, &lines[1][0]);

	//去畸变
	cvUndistortPoints(&sc_imagePoints1, &sc_imagePoints1,M1, D1, 0, M1);
	cvUndistortPoints(&sc_imagePoints2, &sc_imagePoints2,M2, D2, 0, M2);
	cvComputeCorrespondEpilines(&sc_imagePoints1, 1, F, &cce_L1 );
	cvComputeCorrespondEpilines(&sc_imagePoints2, 2, F, &cce_L2 );

	//求误差
    report.AppendLine(" ");
    report.AppendLine("  -- measureing errors --");
	for( j = 0; j < valid_pic_count ; j++ ){ 
		float avgErr=0;
		for( i = 0; i < cor_count ; i++ ) {
			float err = fabs(lr_corner_points[0][i+j*cor_count].x*lines[1][i+j*cor_count].x +
				lr_corner_points[0][i+j*cor_count].y*lines[1][i+j*cor_count].y + 
				lines[1][i+j*cor_count].z)
				+ fabs(lr_corner_points[1][i+j*cor_count].x*lines[0][i+j*cor_count].x +
				lr_corner_points[1][i+j*cor_count].y*lines[0][i+j*cor_count].y +
				lines[0][i+j*cor_count].z);
			avgErr += err;
		}

        report_val = avgErr/cor_count;
        report_line = string("error for foto pair") + IntToStr(j) + string(": ")
            + FloatToStr(report_val) + "(pixel)";
        report.AppendLine(report_line);
	}

    if (pProgressHandle)
    {
        (*pProgressHandle)(100, "搞定。", NULL, false);
    }

    return report.GetText();
}

string StereoCalib2(
    vector<string>& pictures_l,
    vector<string>& pictures_r,
    int chessboard_width,
    int chessboard_height,
    float squaresize,
    double ccdpixelsize,
    CCameralMat& mat,
    progresscallback* pProgressHandle)
{
    sort(pictures_l.begin(), pictures_l.end());
    sort(pictures_r.begin(), pictures_r.end());
    vector<string> pictures;
    for (long i=0; i<pictures_l.size(); i++)
    {
        pictures.push_back(pictures_l[i]);
        pictures.push_back(pictures_r[i]);
    }

    return DoStereoCalib(pictures, chessboard_width, chessboard_height, squaresize,
        ccdpixelsize, mat, pProgressHandle);
};

string getnowstr()
{
    time_t now;
//    time(&now);
    now = time(0);

    struct tm *ptr;
    ptr=localtime(&now); 

    char buffdate[100];
    strftime(buffdate, 100, "%Y", ptr);
    string r = buffdate;
    strftime(buffdate, 100, "%m", ptr);
    r = r + "_" + string(buffdate);
    strftime(buffdate, 100, "%d", ptr);
    r = r + "_" + string(buffdate);
    strftime(buffdate, 100, "%H", ptr);
    r = r + "_" + string(buffdate);
    strftime(buffdate, 100, "%M", ptr);
    r = r + "_" + string(buffdate);
    strftime(buffdate, 100, "%S", ptr);
    r = r + "_" + string(buffdate);
    return r;
}


#if !defined(WINAPI)
    #define WINAPI __stdcall
#endif

#if !defined(DLL)
    #define DLL __declspec(dllexport)
#endif

typedef const char* PChar;

string g_latestcalibresult;
PChar DLL WINAPI StereoCalib(
    const char* leftfotolist, const char* rightfotolist,
    int chessboard_width, int chessboard_height, double squaresize,
    double ccdpixelsize, const char* calibfile, progresscallback* pProgressHandle)
{
    Strings leftlist, rightlist;
    leftlist.SetText(leftfotolist);
    rightlist.SetText(rightfotolist);

    if (leftlist.GetLineCount() != rightlist.GetLineCount())
        return NULL;
    if (leftlist.GetLineCount() == 0)
        return NULL;

    vector<string> pictures_l;
    vector<string> pictures_r;

    for (long i=0; i<leftlist.GetLineCount(); i++)
    {
        string line;
        leftlist.GetLine(i, line);
        pictures_l.push_back(line);
        rightlist.GetLine(i, line);
        pictures_r.push_back(line);
    }

    sort(pictures_l.begin(), pictures_l.end());
    sort(pictures_r.begin(), pictures_r.end());

    string report = StereoCalib2(pictures_l, pictures_r, chessboard_width, chessboard_height,
        squaresize, ccdpixelsize, g_CameraMat, pProgressHandle);
    if (report == "")
        return NULL;

    string parfile;
    if (calibfile)
    {
        parfile = Trim(calibfile);
    }

    if (parfile == "")
    {
        string leftpath = GetDirectoryPart(pictures_l[0]);
        leftpath = Trim(LowerString(GetDirectoryPart(leftpath)));
        string rightpath = GetDirectoryPart(pictures_r[0]);
        rightpath = Trim(LowerString(GetDirectoryPart(rightpath)));
        if ((leftpath == rightpath) && (leftpath != ""))
        {
            parfile = leftpath + "\\" + getnowstr() + ".par";
        }
        else
        {
            parfile = GetDirectoryPart(pictures_l[0]) + "\\" + getnowstr() + ".par";
        }
    }

    g_CameraMat.save(parfile.c_str());

    Strings result;
    result.AppendLine(parfile);
    result.AppendLine(" ");
    result.AppendLine(report);
    g_latestcalibresult = result.GetText();
    return g_latestcalibresult.c_str();
}

PChar DLL WINAPI StereoCalib1(const char* fotolist,
    int chessboard_width, int chessboard_height, double squaresize, double ccdpixelsize,
    const char* calibfile, progresscallback* pProgressHandle)
{
    Strings fotos;
    fotos.SetText(fotolist);

    if (fotos.GetLineCount() == 0)
        return NULL;

    vector<string> pictures;

    for (long i=0; i<fotos.GetLineCount(); i++)
    {
        string line;
        fotos.GetLine(i, line);
        pictures.push_back(line);
    }

    sort(pictures.begin(), pictures.end());
    vector<float> errs;

    string report = DoStereoCalib(pictures, chessboard_width, chessboard_height,
        squaresize, ccdpixelsize, g_CameraMat, pProgressHandle);
    if (report == "")
        return NULL;

    string parfile;
    if (calibfile)
    {
        parfile = Trim(calibfile);
    }

    if (parfile == "")
    {
        parfile = GetDirectoryPart(pictures[0]) + "\\" + getnowstr() + ".par";
    }

    g_CameraMat.save(parfile.c_str());

    Strings result;
    result.AppendLine(parfile);
    result.AppendLine(" ");
    result.AppendLine(report);
    g_latestcalibresult = result.GetText();
    return g_latestcalibresult.c_str();
}