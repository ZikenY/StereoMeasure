// JPGImage.h: interface for the Image class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__629905E8_60A2_4F2A_B70C_CF59F678C42E__INCLUDED_)
#define AFX_IMAGE_H__629905E8_60A2_4F2A_B70C_CF59F678C42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct JPGImage  
{
    int             m_nW,m_nH,m_mode;
    BYTE*           m_pBits;

    const BYTE* Y(int y)const{ return m_pBits+y*m_nW*m_mode; }
    BYTE* Y(int y){ return m_pBits+y*m_nW*m_mode; }

    bool SaveJpg(int quality,const char* filename);
    int SaveJpgMem(int quality,void* mem);

    //重采样缩放
    void ReSample(JPGImage& img)const;

    //1:1显示裁剪
    void ReSample(JPGImage& img,int x,int y)const;

    //两张图像拼接
    void ReSample(const JPGImage& post,JPGImage& img)const;
    void ReSample(const JPGImage& post,JPGImage& img,int x,int y)const;
};

#endif // !defined(AFX_IMAGE_H__629905E8_60A2_4F2A_B70C_CF59F678C42E__INCLUDED_)
