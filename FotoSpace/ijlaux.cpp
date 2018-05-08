#include "ijlaux.h"
#include "ijl.h"
#pragma comment(lib, "ijl15.lib") 

bool testjpg(const char* filename, long& w, long& h, long& channels)
{
    //先读取jpeg文件信息
    JPEG_CORE_PROPERTIES jcprops;
	ijlInit(&jcprops);
    jcprops.JPGFile = filename;
    IJLERR jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS);

    if (IJL_OK != jerr)
    {
        return false;
    }

    channels = jcprops.JPGChannels;
    w = jcprops.JPGWidth;
    h = jcprops.JPGHeight;
    return true;
}

bool loadjpg(const char* filename, byte* pbuff, long& w, long& h)
{
    if (!pbuff)
        return false;

    //先读取jpeg文件信息
    JPEG_CORE_PROPERTIES jcprops;
	ijlInit(&jcprops);
    jcprops.JPGFile = filename;
    IJLERR jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS);

    if (IJL_OK != jerr)
    {
        return false;
    }

    if (jcprops.JPGChannels  != 3)
    {
    }

    //位图信息
    w = jcprops.DIBWidth = jcprops.JPGWidth;
    h = jcprops.DIBHeight = jcprops.JPGHeight;
    jcprops.DIBColor = IJL_BGR;
    jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.DIBWidth, jcprops.DIBChannels);
//    long imgbuffsize = jcprops.DIBWidth * jcprops.DIBHeight * jcprops.DIBChannels;

    //位图缓冲区
    jcprops.DIBBytes = pbuff;

    //读取jpeg数据
    jerr = ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE);
    if (IJL_OK != jerr)
    {
        return false;
    }

    vertical_reverse_rgb(w, h, pbuff);
    return true;
}

void showrgb(HDC dc, RECT rect, POINT origin, double scale,
    int w, int h, BYTE* pBits)
{
    if (!pBits[25] && !pBits[66] && !pBits[75] && !pBits[99] && !pBits[213])
    {
        return; //空图像
    }

    //创建位图信息
    BITMAPINFO bmpinfo;
    memset(&bmpinfo, 0, sizeof(BITMAPINFO));
    bmpinfo.bmiHeader.biSize = 40;
    bmpinfo.bmiHeader.biWidth = w;
    bmpinfo.bmiHeader.biHeight = h;
    bmpinfo.bmiHeader.biBitCount = 24;
    bmpinfo.bmiHeader.biPlanes = 1;

    //设置拉伸模式
    ::SetStretchBltMode(dc, COLORONCOLOR);

    long left = long(rect.left + origin.x*scale);
    long top = long(rect.top + origin.y*scale);

    StretchDIBits(dc, left, top,
        long(w*scale),
        long(h*scale),
        0, 0, w, h, pBits, &bmpinfo,
        DIB_RGB_COLORS,SRCCOPY);
}

const int g_buftt_size = 100000;
BYTE g_buftt[g_buftt_size];
void vertical_reverse_rgb(int _w, int h, BYTE* pBits)
{
    int w = 3*_w;
    BYTE* t = NULL;
    bool newt = false;
    if (w > g_buftt_size)
    {
        t = new BYTE[w];
        newt = true;
    }
    else
    {
        t = g_buftt;
    }

    for (int i=h/2; --i>=0;)
    {
        int j = h-1-i;
        memcpy(t,pBits+i*w,w);
        memcpy(pBits+i*w,pBits+j*w,w);
        memcpy(pBits+j*w,t,w);
    }

    if (newt)
    {
        delete[] t;
    }
}

void vertical_reverse_gray(int w, int h, BYTE* pBits)
{
    BYTE* t = NULL;
    bool newt = false;
    if (w > g_buftt_size)
    {
        t = new BYTE[w];
        newt = true;
    }
    else
    {
        t = g_buftt;
    }

    for (int i=0; i<h/2; i++)
    {
        int j = w*h-(i+1)*w;
        memcpy(t, pBits+i*w, w);
        memcpy(pBits+i*w, pBits+j, w);
        memcpy(pBits+j, t, w);
    }

    if (newt)
    {
        delete[] t;
    }
}