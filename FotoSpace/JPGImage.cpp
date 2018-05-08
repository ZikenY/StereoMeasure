// Image.cpp: implementation of the JPGImage class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "JPGImage.h"
#include "ijl.h"
#include <malloc.h>

//#pragma comment(lib, "ijl15.lib") 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool JPGImage::SaveJpg(int quality,const char* filename)
{
    JPEG_CORE_PROPERTIES image;

	IJLERR er = ijlInit( &image );

    image.jquality = quality;
	image.JPGFile = filename;

	image.DIBBytes = (BYTE*)(m_pBits);
	image.DIBWidth = image.JPGWidth = m_nW;
	image.DIBHeight = image.JPGHeight = m_nH;
    if(m_mode==1)
    {
	    image.DIBColor = IJL_G;
	    image.DIBChannels = 1;
    }
    else
    {
        image.DIBColor = IJL_BGR;
	    image.DIBChannels = 3;
    }

	er = ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE );
    ijlFree(&image);
    return er==IJL_OK;
}

int JPGImage::SaveJpgMem(int quality,void* mem)
{
    int cs;
    JPEG_CORE_PROPERTIES image;

	IJLERR er = ijlInit( &image );

    image.jquality = quality;

	image.DIBBytes = (BYTE*)(m_pBits);
	image.DIBWidth = image.JPGWidth = m_nW;
	image.DIBHeight = image.JPGHeight = m_nH;

    if(m_mode==1)
    {
	    image.DIBColor = IJL_G;
	    image.DIBChannels = 1;
    }
    else
    {
        image.DIBColor = IJL_BGR;
	    image.DIBChannels = 3;
    }

    image.JPGBytes = (BYTE*)mem;
    image.JPGSizeBytes = m_nW*m_nH*m_mode;

	er = ijlWrite( &image, IJL_JBUFF_WRITEWHOLEIMAGE );
    cs = image.JPGSizeBytes;
    ijlFree(&image);
    return er==IJL_OK?cs:0;
}

void JPGImage::ReSample(JPGImage& img,int x,int y)const
{
    int i,cc;
    if(x+img.m_nW>m_nW) x = m_nW-img.m_nW;
    if(y+img.m_nH>m_nH) y = m_nH-img.m_nH;
    if(x<0) x = 0;
    if(y<0) y = 0;
    x *= m_mode;
    cc = m_nW>img.m_nW?img.m_nW:m_nW;
    cc *= m_mode;
    for(i=0; i<img.m_nH; ++i)
    {
        if(i+y>=m_nH)
            break;
        memcpy(img.Y(i),Y(i+y)+x,cc);
    }
}

void JPGImage::ReSample(const JPGImage& post, JPGImage& img,int x,int y)const
{
    if(x>=m_nW)
    {
        post.ReSample(img,x-m_nW,y);
        return;
    }
    if(x+img.m_nW<=m_nW)
    {
        ReSample(img,x,y);
        return;
    }

    int i,cc,dd;
    if(x+img.m_nW>2*m_nW) x = 2*m_nW-img.m_nW;
    if(y+img.m_nH>m_nH) y = m_nH-img.m_nH;
    if(x<0) x = 0;
    if(y<0) y = 0;
    cc = m_nW-x;
    dd = img.m_nW-cc;
    x *= m_mode;
    cc *= m_mode;
    dd *= m_mode;
    
    for(i=0; i<img.m_nH; ++i)
    {
        if(i+y>=m_nH)
            break;

        BYTE *B = img.Y(i);

        memcpy(B,Y(i+y)+x,cc);
        memcpy(B+cc,post.Y(i+y),dd);
    }
}

static void _re_sample(int m,int w,short* I,const BYTE* C,BYTE* B)
{
    int i,j;
    if(m==1)
    {
        for(i=w; --i>=0;)
        {
            j = I[i];
            B[i] = C[j];
        }
    }
    else if(m==3)
    {
        for(i=3*w; (i-=3)>=0;)
        {
            j = 3*I[i/3];
            B[i] = C[j];
            B[i+1] = C[j+1];
            B[i+2] = C[j+2];
        }
    }
}

void JPGImage::ReSample(JPGImage& img)const
{
    short* I; int iy,x,y;
    static short IA[4096];

    I = IA;
    for(x=0; x<img.m_nW; ++x)
    {
        I[x] = (short)(x*m_nW/img.m_nW);
        if(I[x]>=m_nW) I[x] = m_nW-1;
    }
    
    for(y=0; y<img.m_nH; ++y)
    {
        iy = y*m_nH/img.m_nH;
        if(iy>=m_nH) iy=m_nH-1;
        _re_sample(m_mode,img.m_nW,I,Y(iy),img.Y(y));
    }
}

void JPGImage::ReSample(const JPGImage& post, JPGImage& img)const
{
    int w,n,y; BYTE *P;
    JPGImage A,B,C;
    C = B = A = img;
    w = img.m_nW*img.m_mode;
    n = w*img.m_nH;
    A.m_pBits = new BYTE[4*n];
    B.m_pBits = A.m_pBits+n;
    C.m_pBits = B.m_pBits+n;
    C.m_nW *= 2;
    ReSample(A);
    post.ReSample(B);
    for(y=img.m_nH; --y>=0;)
    {
        P = C.Y(y);
        memcpy(P,A.Y(y),w);
        memcpy(P+w,B.Y(y),w);
    }
    C.ReSample(img);
    delete A.m_pBits;
}
