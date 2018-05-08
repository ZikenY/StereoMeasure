#if !defined(IJLAUX_20130806__INCLUDED_)
#define IJLAUX_20130806__INCLUDED_

#include "windows.h"

bool testjpg(const char* filename, long& w, long& h, long& channels);
bool loadjpg(const char* filename, byte* pbuff, long& w, long& h);

//��dc����ʾλͼ
void showrgb(HDC dc, RECT rect, POINT origin, double scale,
    int w, int h, BYTE* pBits);

//����תͼ��
void vertical_reverse_rgb(int _w, int h, BYTE* pBits);
void vertical_reverse_gray(int w, int h, BYTE* pBits);

#endif