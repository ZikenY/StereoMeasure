#include "cv.h"
#include "highgui.h"

bool PointInRect(const CvPoint& point, const CvRect& rect)
{
    if ((point.x < rect.x)
        || (point.y < rect.y)
        || (point.x > rect.x+rect.width)
        || (point.y > rect.y+rect.height))
    {
        return false;
    }
    return true;
}

bool PointInSize(const CvPoint& point, const CvSize& size)
{
    CvRect rect;
    rect.x = rect.y = 0;
    rect.width = size.width;
    rect.height = size.height;
    return PointInRect(point, rect);
}

bool GetSubExtent(const CvSize& srcsize, const CvSize& subsize,
    const CvPoint& srchomopoint, CvRect& subrect, CvPoint& subhomopoint)
{
    if (!PointInSize(srchomopoint, srcsize))
        return false;

    subrect.width = subsize.width;
    subrect.height = subsize.height;
    subrect.x = srchomopoint.x - subrect.width/2;
    subrect.y = srchomopoint.y - subrect.height/2;
    CvPoint homooffset; homooffset.x = homooffset.y = 0;  //同名点对于匹配子图中心的偏移
    if (subrect.x < 0)
    {
        homooffset.x = subrect.x;
        subrect.x = 0;
    }
    else if (subrect.x > (srcsize.width - subrect.width))
    {
        homooffset.x = subrect.x - (srcsize.width - subrect.width);
        subrect.x = srcsize.width - subrect.width;
    }

    if (subrect.y < 0)
    {
        homooffset.y = subrect.y;
        subrect.y = 0;
    }
    else if (subrect.y > (srcsize.height - subrect.height))
    {
        homooffset.y = subrect.y - (srcsize.height - subrect.height);
        subrect.y = srcsize.height - subrect.height;
    }


    subhomopoint.x = subrect.width/2 + homooffset.x;
    subhomopoint.y = subrect.height/2 + homooffset.y;
    return true;
}

const long MATCHINGBOX1_WIDTH = 161;
const long MATCHINGBOX1_HEIGHT = 161;
const long MATCHINGBOX2_WIDTH = 31;
const long MATCHINGBOX2_HEIGHT = 31;

IplImage* GetSubFoto(IplImage* srcfoto, const CvSize& subsize,
    const CvPoint& srchomopoint, CvPoint& subhomopoint)
{
    CvRect subrect;//匹配用子图
    CvSize srcsize = cvGetSize(srcfoto);
    if (!GetSubExtent(srcsize, subsize, srchomopoint, subrect, subhomopoint))
        return NULL;

    cvSetImageROI(srcfoto, subrect);
    IplImage* subfoto = cvCreateImage(cvGetSize(srcfoto), srcfoto->depth,
        srcfoto->nChannels);
    cvCopy(srcfoto, subfoto, NULL);
    cvResetImageROI(srcfoto);
    return subfoto;
}

IplImage* GetThirdFoto(IplImage* srcfoto, const CvPoint& homotobe, CvPoint& suborigin,
    bool accurate)
{
    CvSize srcsize = cvGetSize(srcfoto);
    CvSize subsize;

    subsize.width = srcsize.width;
    if (accurate || srcsize.height<600)
    {
        subsize.height = srcsize.height;
    }
    else if (srcsize.height<1200)
    {
        subsize.height = srcsize.height/2;
    }
    else
    {
        subsize.height = srcsize.height/4;
    }

    CvRect subrect;
    CvPoint pntxx;
    if (!GetSubExtent(srcsize, subsize, homotobe, subrect, pntxx))
        return NULL;

    suborigin.x = subrect.x;
    suborigin.y = subrect.y;

    cvSetImageROI(srcfoto, subrect);
    IplImage* subfoto = cvCreateImage(cvGetSize(srcfoto), srcfoto->depth,
        srcfoto->nChannels);
    cvCopy(srcfoto, subfoto, NULL);
    cvResetImageROI(srcfoto);
    return subfoto;
}


bool MatchHomoZone(IplImage* srcfoto, IplImage* targetfoto, const CvRect& homorectsrc,
    CvRect& homorecttarget)
{
    CvSize rashsrcsize;
    rashsrcsize.width = homorectsrc.width;
    rashsrcsize.height = homorectsrc.height;
    CvPoint homopntsrc = cvPoint(homorectsrc.x + homorectsrc.width/2, homorectsrc.y + homorectsrc.height/2);
    //取出src上的匹配块
    CvPoint rashhomopoint;//!!!!!!!!!!!!!!!!!!!!!!!!!!
    IplImage* rashsrcfoto = GetSubFoto(srcfoto, rashsrcsize, homopntsrc, rashhomopoint);
    if (!rashsrcfoto)
        return false;

    //matching
    CvSize targetsize = cvGetSize(targetfoto);
    CvSize sizeResult = cvSize(targetsize.width - rashsrcsize.width + 1,
        targetsize.height - rashsrcsize.height + 1); 

    IplImage* imgResult = cvCreateImage(sizeResult, IPL_DEPTH_32F, 1); 
    IplImage* targetgray = cvCreateImage(cvGetSize(targetfoto), 8, 1);
    IplImage* GrayTemp = cvCreateImage(cvGetSize(rashsrcfoto), 8, 1);
    cvCvtColor(targetfoto, targetgray, CV_BGR2GRAY);
    cvCvtColor(rashsrcfoto, GrayTemp, CV_BGR2GRAY);
    CvPoint min_loc;
    CvPoint max_loc;
    double min_val;
    double max_val;
    cvMatchTemplate(targetgray, GrayTemp, imgResult, CV_TM_CCORR_NORMED);
    cvMinMaxLoc(imgResult,&min_val, &max_val, &min_loc, &max_loc, NULL); 
    homorecttarget = cvRect(max_loc.x, max_loc.y, rashsrcfoto->width, rashsrcfoto->height);

    cvReleaseImage(&imgResult);
    cvReleaseImage(&targetgray);
    cvReleaseImage(&GrayTemp);
    cvReleaseImage(&rashsrcfoto);
    return true;
}

inline CvPoint GetCenterPoint(const CvRect &rect)
{
    return cvPoint(rect.x + rect.width/2, rect.y + rect.height/2);
}

inline CvSize GetSizeOfRect(const CvRect &rect)
{
    return cvSize(rect.width, rect.height);
}

inline CvRect BuildRect(const CvPoint& center, const long width, const long height)
{
    return cvRect(center.x - width/2, center.y - height/2, width, height);
}

bool DoMatchHomoPoint(IplImage* srcfoto, IplImage* targetfoto, const CvPoint& homopntsrc,
    CvPoint& homopnttarget, bool accurate)
{
    //扣出target foto的三分之一或二分之一或1
    CvPoint thirdtargetorigin;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    IplImage* thirdtargetfoto = GetThirdFoto(targetfoto, homopntsrc, thirdtargetorigin, accurate);

    if (!thirdtargetfoto)
        return false;

    //初步匹配srcfoto与thirdtargetfoto上的同名区域
    CvRect srcrect = BuildRect(homopntsrc, MATCHINGBOX1_WIDTH, MATCHINGBOX1_HEIGHT);
    CvRect targetrect1;
    if (!MatchHomoZone(srcfoto, thirdtargetfoto, srcrect, targetrect1))
    {
        cvReleaseImage(&thirdtargetfoto);
        return false;
    }

    //精确匹配
    CvPoint precisetargetorigin;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    IplImage* precisetargetfoto = GetSubFoto(thirdtargetfoto, GetSizeOfRect(targetrect1),
        GetCenterPoint(targetrect1), precisetargetorigin);
    srcrect = BuildRect(homopntsrc, MATCHINGBOX2_WIDTH, MATCHINGBOX2_HEIGHT);
    CvRect targetrect2;
    if (!MatchHomoZone(srcfoto, precisetargetfoto, srcrect, targetrect2))
    {
        cvReleaseImage(&thirdtargetfoto);
        cvReleaseImage(&precisetargetfoto);
        return false;
    }

    //现在把targetrect2回溯到targetfoto上
    targetrect2.x += targetrect1.x;
    targetrect2.y += targetrect1.y;
    targetrect2.x += thirdtargetorigin.x;
    targetrect2.y += thirdtargetorigin.y;
    homopnttarget = GetCenterPoint(targetrect2);

    cvReleaseImage(&thirdtargetfoto);
    cvReleaseImage(&precisetargetfoto);
    return true;
}

bool __declspec(dllexport) __stdcall MatchHomoPointWithFotos(const char* srcfoto, const char* targetfoto,
    const CvPoint homopntsrc, CvPoint& homopnttarget, bool accurate)
{
    IplImage* srcimg = cvLoadImage(srcfoto, 1);
    if (!srcimg)
        return false;

    IplImage* targetimg = cvLoadImage(srcfoto, 1);
    if (!targetimg)
    {
        cvReleaseImage(&srcimg);
        return false;
    }

    bool r = DoMatchHomoPoint(srcimg, targetimg, homopntsrc, homopnttarget, accurate);
    cvReleaseImage(&srcimg);
    cvReleaseImage(&targetimg);
    return r;
}


struct killall
{
    IplImage* _srcimg;
    IplImage* _targetimg;
    void Release()
    {
        if (_srcimg)
            _srcimg = NULL;
        if (_targetimg)
            _targetimg = NULL;
    }
    killall()
    {
        _srcimg = NULL;
        _targetimg = NULL;
    };
    ~killall()
    {
        this->Release();
    };
};killall k;

bool __declspec(dllexport) __stdcall SetMatchFotos(const char* srcfoto, const char* targetfoto)
{
    k.Release();

    k._srcimg = cvLoadImage(srcfoto, 1);
    k._targetimg = cvLoadImage(targetfoto, 1);
    if (!k._srcimg || !k._targetimg)
    {
        k.Release();
        return false;
    }

    return true;
}

bool __declspec(dllexport) __stdcall MatchHomoPoint(const CvPoint homopntsrc, CvPoint& homopnttarget,
    bool swapfotos, bool accurate)
{
    if (!k._srcimg || !k._targetimg)
    {
        k.Release();
        return false;
    }

    if (!swapfotos)
    {
        return DoMatchHomoPoint(k._srcimg, k._targetimg, homopntsrc, homopnttarget, accurate);
    }
    else
    {
        return DoMatchHomoPoint(k._targetimg, k._srcimg, homopntsrc, homopnttarget, accurate);
    }
}