#if !defined(__CAMERALMAT_H__)
#define __CAMERALMAT_H__
#pragma warning (disable:4793)
#include <cv.h>

struct CCameralMat
{
    CvMat* M1;  //左相机内参数矩阵
	CvMat* M2;  //右
	CvMat* D1;  //左相机畸变参数
	CvMat* D2;  //右
	CvMat* R;   //旋转矩阵
	CvMat* T ;  //平移向量
	CvMat* E ;  //本征矩阵
	CvMat* F ;  //基础矩阵
    float avgErr;

    //用于矫正左右图像的查找映射表
    CvMat* mx1;
    CvMat* my1;
    CvMat* mx2;
    CvMat* my2;
    CvSize imageSize;

    //用于立体匹配的bm结构
    CvStereoBMState* bmstate;
    CvMat* disp;

    CCameralMat();
	CCameralMat(const CCameralMat& mat);
	~CCameralMat();
	void operator=(CCameralMat& mat);

    bool RecalifyImages(const char* ori_left, const char* ori_right,
        const char* cal_left, const char* cal_right);

    bool UndistortFoto(const char* ori, const char* undistorted, bool isleft);

    void save(const char *filePath);
    bool read(const char *filePath);

private:
    void init();
    void ReleaseMaps();
    void CalcRecalifiedMaps(const CvSize& newsize);
};

static CCameralMat g_CameraMat;

#endif