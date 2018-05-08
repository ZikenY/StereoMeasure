#if !defined(__CAMERALMAT_H__)
#define __CAMERALMAT_H__
#pragma warning (disable:4793)
#include <cv.h>

struct CCameralMat
{
    CvMat* M1;  //������ڲ�������
	CvMat* M2;  //��
	CvMat* D1;  //������������
	CvMat* D2;  //��
	CvMat* R;   //��ת����
	CvMat* T ;  //ƽ������
	CvMat* E ;  //��������
	CvMat* F ;  //��������
    float avgErr;

    //���ڽ�������ͼ��Ĳ���ӳ���
    CvMat* mx1;
    CvMat* my1;
    CvMat* mx2;
    CvMat* my2;
    CvSize imageSize;

    //��������ƥ���bm�ṹ
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