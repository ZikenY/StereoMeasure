#include "FotoSpace.h"
#include "ijlaux.h"
#include "JPGImage.h"
#include "..\\wplib\\StringFuncs.h"
using namespace weipan;

const char* THUMBNAIL_DIR = "\\~fotospacethumbnail";

CFotoSpace::CFotoSpace()
{
    ::memset(_buff, 0, sizeof(_buff));
    _hwnd = NULL;
    _origin.x = 0;
    _origin.y = 0;
    _scale = 1;
    _bkgcolor = RGB(0,0,0);
//    _measureline = false;
    _panstarted = false;
    _nextfid = 0;
    _selectedfid = -1;
    _w = _h = 1;
}

CFotoSpace::~CFotoSpace()
{
}

long CFotoSpace::_screen2pic_x(long x)
{
    return x/_scale - _origin.x;
}

long CFotoSpace::_screen2pic_y(long y)
{
    return y/_scale - _origin.y;
}

long CFotoSpace::_pic2screen_x(long x)
{
    return (x + _origin.x)*_scale;
}

long CFotoSpace::_pic2screen_y(long y)
{
    return (y + _origin.y)*_scale;
}

bool CFotoSpace::_setscale(double scale)
{
    RECT rect;
    ::GetClientRect(_hwnd, &rect);
    double zoomall_scale = double(rect.right-rect.left) / _h;
    double window_radio = (rect.right-rect.left) / (rect.bottom-rect.top);
    double src_ratio = (_w*1.0)/(_h*1.0);
    if (window_radio < src_ratio)
    {
        zoomall_scale = double(rect.right-rect.left) / _w;
    }

    if ((scale < (zoomall_scale - zoomall_scale/100.0))
        && (scale < _scale))
    {
        return false;
    }
    else if ((scale > zoomall_scale*100)
        && (scale > _scale))
    {
        return false;
    }

    long old_left       = _screen2pic_x(rect.left);
    long old_top        = _screen2pic_y(rect.top);
    long old_right      = _screen2pic_x(rect.right);
    long old_bottom     = _screen2pic_y(rect.bottom);

    _scale = scale;

    _origin.x += ((_screen2pic_x(rect.right) - _screen2pic_x(rect.left)) - (old_right - old_left)) / 2;
    _origin.y += ((_screen2pic_x(rect.bottom) - _screen2pic_x(rect.top)) - (old_bottom - old_top)) / 2;
    return true;
}

void CFotoSpace::_zoomall()
{
    RECT rect;
    ::GetClientRect(_hwnd, &rect);
    double window_radio = (rect.right-rect.left) / (rect.bottom-rect.top);
    double src_ratio = (_w*1.0)/(_h*1.0);

    if (window_radio < src_ratio)
    {
        _scale = double(rect.right-rect.left) / _w;
        _origin.y = (_screen2pic_y(rect.bottom) - _screen2pic_y(rect.top))/2 - _h/2;
        _origin.x = 0;
    }
    else
    {
        _scale = double(rect.bottom-rect.top) / _h;
        _origin.x = (_screen2pic_x(rect.right) - _screen2pic_x(rect.left))/2 - _w/2;
        _origin.y = 0;
    }
}

void CFotoSpace::_panstart(long x, long y)
{
    _panpntstart.x = x;
    _panpntstart.y = y;
    _panpntto = _panpntstart;
    _panstarted = true;
}

void CFotoSpace::_panmoveTo(long x, long y)
{
    HDC dc = ::GetDC(_hwnd);
    RECT rect;
    ::GetClientRect(_hwnd, &rect);

    //用位图来做
    HDC blankdc = ::CreateCompatibleDC(dc);
    HBITMAP bitmap = ::CreateCompatibleBitmap(dc, rect.right-rect.left, rect.bottom-rect.top);
    ::SelectObject(blankdc, bitmap);
    ::DeleteObject(bitmap);

    //黑黑一片
    HBRUSH blankbrush = ::CreateSolidBrush(_bkgcolor);
    ::FillRect(blankdc, &rect, blankbrush);
    ::DeleteObject(blankbrush);

    //先把偏移过的飞机贴到blankdc上
    RECT rect_moved;
    rect_moved.left = x - _panpntstart.x;
    rect_moved.top = y - _panpntstart.y;
    rect_moved.right = x + rect.right-rect.left;
    rect_moved.bottom = y + rect.bottom-rect.top;
    showrgb(blankdc, rect_moved, _origin, _scale, _w, _h, _buff);

    //再把blankdc上的内容贴到窗体DC上
    BitBlt(dc, 0, 0, rect.right-rect.left, rect.bottom-rect.top,
        blankdc, 0, 0, SRCCOPY);

    _panpntto.x = x;
    _panpntto.y = y;

    ::DeleteDC(blankdc);
    ::ReleaseDC(this->_hwnd, dc);
}

void CFotoSpace::_panstop()
{
    if (abs(_panpntto.x-_panpntstart.x)<1 || abs(_panpntto.y-_panpntstart.y)<1)
        return;

    long delta_x = _screen2pic_x(_panpntto.x) - _screen2pic_x(_panpntstart.x);
    long delta_y = _screen2pic_y(_panpntto.y) - _screen2pic_y(_panpntstart.y);
    _origin.x += delta_x;
    _origin.y += delta_y;
    _panstarted = false;
    _updatespace();
}

void CFotoSpace::_updatespace()
{
    if (!_hwnd)
        return;

    HDC dc = ::GetDC(_hwnd);
    HDC buffdc = ::CreateCompatibleDC(dc);
    RECT rect;
    ::GetClientRect(_hwnd, &rect);

    //双缓冲方式，先画在buffdc上
    HBITMAP bitmap = ::CreateCompatibleBitmap(dc, rect.right-rect.left,
        rect.bottom-rect.top);
    ::SelectObject(buffdc, bitmap);
    ::DeleteObject(bitmap);

    //擦除背景
    HBRUSH brush = ::CreateSolidBrush(_bkgcolor);
    ::FillRect(buffdc, &rect, brush);
    ::DeleteObject(brush);

    //画jpg pic
    showrgb(buffdc, rect, _origin, _scale, _w, _h, _buff);

    //-------------------- 准备画笔环境 ---------------------
    HPEN pen = ::CreatePen(PS_SOLID, 2, RGB(0,255,0));
    ::SelectObject(buffdc, pen);
    brush = ::CreateSolidBrush(RGB(255,0,0));
    ::SelectObject(buffdc, brush);

    LOGFONT logfont;
    ::memset(&logfont, 0, sizeof(LOGFONT));
    ::strcpy(logfont.lfFaceName, "Times New Roman");
    logfont.lfHeight = 15;
    logfont.lfWidth = 6;
    HFONT font = ::CreateFontIndirect(&logfont);
    ::SelectObject(buffdc, font);
    ::SetBkMode(buffdc, OPAQUE);
    ::SetTextColor(buffdc, RGB(45, 10, 25));
    ::SetROP2(buffdc, R2_COPYPEN);
    UINT oldalign = ::GetTextAlign(buffdc);
//    ::SetTextAlign(buffdc, TA_CENTER);
    //-------------------- 准备画笔环境 ---------------------


    //-------------------- draw geometries ---------------------
    long select_index = -1;
    for (long i=0; i<_lines.size(); i++)
    {
        if (_lines[i].pnts.size() <= 0)
        {
            continue;
        }

        if (_selectedfid == _lines[i].fid)
        {
            select_index = i;
            continue;   //选中的不在这里画
        }

        this->_drawsingleline(_lines[i], buffdc);
    }

    //最后画选中的
    if (select_index >= 0)
    {
        _drawselection(buffdc, rect, _lines[select_index]);
    }
    //-------------------- draw geometries ---------------------

    ::BitBlt(dc, rect.left, rect.top, rect.right-rect.left,
        rect.bottom-rect.top, buffdc, 0, 0, SRCCOPY);

    ::DeleteObject(font);
    ::DeleteObject(brush);
    ::DeleteObject(pen);

    ::DeleteDC(buffdc);
    ::ReleaseDC(this->_hwnd, dc);

    _refreshmeasureline();
}

void CFotoSpace::_drawsingleline(CFotoLine line, HDC dc)
{
    SIZE size;
    ::GetTextExtentPoint32(dc, line.label.c_str(), line.label.size(), &size);

    POINT pnt1, pnt2, labelpnt;
    if ((line.pnts.size() == 1) ||
        (abs(line.pnts[0].x - line.pnts[1].x) < 2
        && abs(line.pnts[0].y - line.pnts[1].y) < 2))
    {
        //点
        pnt1.x = _pic2screen_x(line.pnts[0].x);
        pnt1.y = _pic2screen_y(line.pnts[0].y);
        ::Ellipse(dc, pnt1.x-4, pnt1.y-4, pnt1.x+4, pnt1.y+4);

        labelpnt.x = pnt1.x;
        labelpnt.y = pnt1.y;
        labelpnt.x -= size.cx/2;
        labelpnt.y = labelpnt.y + size.cy/2;
        ::TextOut(dc, labelpnt.x, labelpnt.y, line.label.c_str(), line.label.size());
    }
    else if (line.pnts.size() == 2)
    {
        //线
        pnt1.x = _pic2screen_x(line.pnts[0].x);
        pnt1.y = _pic2screen_y(line.pnts[0].y);
        pnt2.x = _pic2screen_x(line.pnts[1].x);
        pnt2.y = _pic2screen_y(line.pnts[1].y);

        ::MoveToEx(dc, pnt1.x, pnt1.y, NULL);
        ::LineTo(dc, pnt2.x, pnt2.y);
        ::Ellipse(dc, pnt1.x-2, pnt1.y-2, pnt1.x+2, pnt1.y+2);
        ::Ellipse(dc, pnt2.x-2, pnt2.y-2, pnt2.x+2, pnt2.y+2);

        labelpnt.x = (pnt1.x+pnt2.x)/2;
        labelpnt.y = (pnt1.y+pnt2.y)/2;
        labelpnt.x -= size.cx/2;
        labelpnt.y -= size.cy/2;
        ::TextOut(dc, labelpnt.x, labelpnt.y, line.label.c_str(), line.label.size());
    }
    else
    {
        //region
        this->_drawregion(line, size, dc);
    }
}

void CFotoSpace::_drawregion(CFotoLine region, SIZE labelsize, HDC dc)
{
    POINT pnt1, pnt2, labelpnt;
    pnt1.x = _pic2screen_x(region.pnts[0].x);
    pnt1.y = _pic2screen_y(region.pnts[0].y);
    vector<POINT>::const_iterator it = region.pnts.begin();
    labelpnt = pnt1;
    it++;
    while(it != region.pnts.end())
    {

        pnt2.x = _pic2screen_x(it->x);
        pnt2.y = _pic2screen_y(it->y);

        ::MoveToEx(dc, pnt1.x, pnt1.y, NULL);
        ::LineTo(dc, pnt2.x, pnt2.y);
        ::Ellipse(dc, pnt1.x-2, pnt1.y-2, pnt1.x+2, pnt1.y+2);
        ::Ellipse(dc, pnt2.x-2, pnt2.y-2, pnt2.x+2, pnt2.y+2);

        labelpnt.x += pnt2.x;
        labelpnt.y += pnt2.y;

        pnt1 = pnt2;
        it++;
    }

    ::MoveToEx(dc, pnt1.x, pnt1.y, NULL);
    ::LineTo(dc, _pic2screen_x(region.pnts[0].x), _pic2screen_y(region.pnts[0].y));

    labelpnt.x = labelpnt.x / region.pnts.size();
    labelpnt.y = labelpnt.y / region.pnts.size();
    labelpnt.x -= labelsize.cx/2;
    labelpnt.y -= labelsize.cy/2;
    ::TextOut(dc, labelpnt.x, labelpnt.y, region.label.c_str(), region.label.size());
}

void CFotoSpace::_drawselection(HDC buffdc, RECT rect, CFotoLine& line)
{
    HPEN pen = ::CreatePen(PS_SOLID, 3, RGB(230,130,220));
    ::SelectObject(buffdc, pen);
    HBRUSH brush = ::CreateSolidBrush(RGB(255,255,0));
    ::SelectObject(buffdc, brush);

    LOGFONT logfont;
    ::memset(&logfont, 0, sizeof(LOGFONT));
    ::strcpy(logfont.lfFaceName, "Times New Roman");
    logfont.lfHeight = 17;
    logfont.lfWidth = 7;
    HFONT font = ::CreateFontIndirect(&logfont);
    ::SelectObject(buffdc, font);
    ::SetBkMode(buffdc, OPAQUE);
    ::SetTextColor(buffdc, RGB(10, 45, 15));
    ::SetROP2(buffdc, R2_COPYPEN);
    UINT oldalign = ::GetTextAlign(buffdc);
//    ::SetTextAlign(buffdc, TA_CENTER);

    this->_drawsingleline(line, buffdc);

    ::DeleteObject(font);
    ::DeleteObject(brush);
    ::DeleteObject(pen);
}

void CFotoSpace::_refreshmeasureline()
{
    if (_measuresegments.size() == 0)
    {
        return;
    }
    else if (_measuresegments.size() == 1)
    {
        this->_drawmeasuresegments(_measuresegments[0], _measuresegments[0]);
        return;
    }

    POINT _measurelinestart = _measuresegments[0];
    vector<POINT>::const_iterator it = _measuresegments.begin();
    it++;
    while (it != _measuresegments.end())
    {
        POINT _measurelineto = *it;
        this->_drawmeasuresegments(_measurelinestart, _measurelineto);
        _measurelinestart = _measurelineto;
        it++;
    }

    this->_drawmeasuresegments(_measurelinestart, _measuresegments[0]);
}

void CFotoSpace::_drawmeasuresegments(POINT _measurelinestart, POINT _measurelineto)
{
    long pnt1_x = _pic2screen_x(_measurelinestart.x);
    long pnt1_y = _pic2screen_y(_measurelinestart.y);
    long pnt2_x = _pic2screen_x(_measurelineto.x);
    long pnt2_y = _pic2screen_y(_measurelineto.y);

    HDC dc = ::GetDC(_hwnd);

    HPEN pen = ::CreatePen(PS_SOLID, 2, RGB(190,45,169));
    HPEN pensaved = (HPEN)::SelectObject(dc, pen);

    if (abs(_measurelinestart.x - _measurelineto.x) < 2
        && abs(_measurelinestart.y - _measurelineto.y) < 2)
    {
        //point
        ::Ellipse(dc, pnt1_x-4, pnt1_y-4, pnt1_x+4, pnt1_y+4);
    }
    else
    {
        //line
        ::MoveToEx(dc, pnt1_x, pnt1_y, NULL);
        ::LineTo(dc, pnt2_x, pnt2_y);
        ::Ellipse(dc, pnt1_x-3, pnt1_y-3, pnt1_x+3, pnt1_y+3);
        ::Ellipse(dc, pnt2_x-3, pnt2_y-3, pnt2_x+3, pnt2_y+3);
    }

    ::SelectObject(dc, pensaved);
    ::DeleteObject(pen);
    ::ReleaseDC(this->_hwnd, dc);
}

void CFotoSpace::_drawmeasureline(long pnt1_x, long pnt1_y, long pnt2_x,
    long pnt2_y)
{
    _measuresegments.clear();
    POINT pnt;
    pnt.x = _screen2pic_x(pnt1_x);
    pnt.y = _screen2pic_y(pnt1_y);
    _measuresegments.push_back(pnt);
    pnt.x = _screen2pic_x(pnt2_x);
    pnt.y = _screen2pic_y(pnt2_y);
    _measuresegments.push_back(pnt);
    _updatespace();
}

bool CFotoSpace::_getmeasureline(long& start_x, long& start_y, long& to_x,
    long& to_y)
{
    if (_measuresegments.size() < 2)
        return false;

    start_x = _measuresegments[0].x;
    start_y = _measuresegments[0].y;
    to_x    = _measuresegments[1].x;
    to_y    = _measuresegments[1].y;
    return true;
}

long CFotoSpace::_measure2line(char* label)
{
    long fid = -1;
    if (_measuresegments.size() < 1)
        return -1;

    if (_measuresegments.size() == 1)
    {
        POINT pnt;
        pnt = _measuresegments[0];
        _measuresegments.push_back(pnt);
    }

    CFotoLine line(_nextfid++, FOTOMEASURELINE);
    if (_measuresegments.size() > 2)
    {
        line.suckit = FOTOMEASURERIGION;
    }

    vector<POINT>::const_iterator it = _measuresegments.begin();
    while (it != _measuresegments.end())
    {
        POINT pnt = *it;
        line.pnts.push_back(pnt);
        it++;
    }

    line.label = label;
    fid = line.fid;
    _lines.push_back(line);

    return fid;
}

void CFotoSpace::_addmeasuresegment(long x, long y)
{
    POINT pnt;
    pnt.x = _screen2pic_x(x);
    pnt.y = _screen2pic_y(y);
    _measuresegments.push_back(pnt);
}

bool CFotoSpace::_getmeasuresegment(long index, long& x, long& y)
{
    if (index < 0)
        return false;
    if (index >= _measuresegments.size())
        return false;

    x = _measuresegments[index].x;
    y = _measuresegments[index].y;
    return true;
}

long CFotoSpace::_getmeasuresegmentcount()
{
    return _measuresegments.size();
}

bool CFotoSpace::_removelastmeasuresegment()
{
    _measuresegments.pop_back();
    return (_measuresegments.size() != 0);
}

void CFotoSpace::_clearmeasureline()
{
    _measuresegments.clear();
    _updatespace();
}

void CFotoSpace::_sethwnd(HWND hwnd)
{
    this->_hwnd = hwnd;
}

bool CFotoSpace::_loadjpg(char* pcfilename, bool genthumb)
{
    this->_clearall();

    if (!loadjpg(pcfilename, _buff, _w, _h))
    {
        ::memset(_buff, 0, sizeof(_buff));
        return false;
    }

    _filename = pcfilename;

    if (genthumb)
        _generatethumbnail(false);

    return true;
}

void CFotoSpace::_clearall()
{
    this->_cleargeometries();
    ::memset(_buff, 0, sizeof(_buff));
}

long CFotoSpace::_addline(long pnt1_x, long pnt1_y, long pnt2_x, long pnt2_y,
    char* label)
{
    CFotoLine line(_nextfid++, FOTOMEASURELINE);
    POINT pnt;
    pnt.x = _screen2pic_x(pnt1_x);
    pnt.y = _screen2pic_y(pnt1_y);
    line.pnts.push_back(pnt);
    pnt.x = _screen2pic_x(pnt2_x);
    pnt.y = _screen2pic_y(pnt2_y);
    line.pnts.push_back(pnt);
    line.label = label;
    _lines.push_back(line);
    return line.fid;
}

long CFotoSpace::_addline(long pnt1_x, long pnt1_y, long pnt2_x, long pnt2_y,
    double distance)
{
    return _addline(pnt1_x, pnt1_y, pnt2_x, pnt2_y, (char*)FloatToStr(distance).c_str());
}

void CFotoSpace::_cleargeometries()
{
    _lines.clear();
    _selectedfid = -1;
}

struct Rect
{
    double left;
    double top;
    double right;
    double bottom;
};

bool PointInEnvelope(const double& PointX, const double& PointY,
    const Rect& env)
{
    if (PointX < env.left) return false;
    if (PointX > env.right) return false;
    if (PointY > env.bottom) return false;
    if (PointY < env.top) return false;
    return true;
}

bool rect_on_line(long from_x, long from_y, long to_x, long to_y,
    Rect rect)
{
    if (PointInEnvelope(from_x, from_y, rect))
        return true;

    if (PointInEnvelope(to_x, to_y, rect))
        return true;

    return false;
}

bool rect_on_Line(Rect rect, const CFotoSpace::CFotoLine& line)
{
    vector<POINT>::const_iterator it = line.pnts.begin();
    while (it != line.pnts.end())
    {
        if (PointInEnvelope(it->x, it->y, rect))
        {
            return true;
        }

        it++;
    }
    
    return false;
}

long CFotoSpace::_selectbypoint(long pnt_x, long pnt_y)
{
    Rect rect;
    rect.left = _screen2pic_x(pnt_x-10);
    rect.top = _screen2pic_y(pnt_y-10);
    rect.right = _screen2pic_x(pnt_x+10);
    rect.bottom = _screen2pic_y(pnt_y+10);

    vector<CFotoLine>::iterator it = _lines.begin();
    while (it != _lines.end())
    {
        if (rect_on_Line(rect, *it))
        {
            _selectedfid = (*it).fid;
            return _selectedfid;
        }

        it++;
    }

    return -1;
}

bool CFotoSpace::_selectbyid(long fid)
{
    _selectedfid = -1;

    for (long i=0; i<_lines.size(); i++)
    {
        if (fid == _lines[i].fid)
        {
            _selectedfid = fid;
            return true;
        }
    }

    return false;
}

long CFotoSpace::_getselect(long& start_x, long& start_y, long& to_x,
    long& to_y, char** label)
{
    vector<CFotoLine>::iterator it = _lines.begin();
    while (it != _lines.end())
    {
        if (_selectedfid == (*it).fid)
        {
            if ((*it).pnts.size() == 0)
            {
                return (*it).fid;
            }
            else if ((*it).pnts.size() == 1)
            {
                start_x = (*it).pnts[0].x;
                start_y = (*it).pnts[0].y;
                to_x = (*it).pnts[0].x;
                to_y = (*it).pnts[0].y;
            }
            else
            {
                start_x = (*it).pnts[0].x;
                start_y = (*it).pnts[0].y;
                to_x = (*it).pnts[1].x;
                to_y = (*it).pnts[1].y;
            }

            if (label)
            {
                *label = (char*)(*it).label.c_str();
            }

            return (*it).fid;
        }

        it++;
    }

    return -1;
}

void CFotoSpace::_deleteselection()
{
    vector<CFotoLine>::iterator it = _lines.begin();
    while (it != _lines.end())
    {
        if (_selectedfid == (*it).fid)
        {
            _lines.erase(it);
            _selectedfid = -1;
            return;
        }

        it++;
    }
}

void CFotoSpace::_deselect()
{
    _selectedfid = -1;
}

long CFotoSpace::_deletelast()
{
    _selectedfid = -1;
    long latestfid = -1;
    vector<CFotoLine>::iterator it_delete = _lines.end();

    vector<CFotoLine>::iterator it = _lines.begin();
    while (it != _lines.end())
    {
        if (latestfid < (*it).fid)
        {
            latestfid = (*it).fid;
            it_delete = it;
        }
        it++;
    }

    if (it_delete != _lines.end())
    {
         _lines.erase(it_delete);
    }

    return latestfid;
}

bool CFotoSpace::_savemeasures(char* filename)
{
    string fname;
    if (filename)
    {
        fname = filename;
    }
    else
    {
        fname = RemoveExtNamePart(_filename) + ".fsm";
    }

    Strings lines;
    lines.AppendLine(IntToStr(_lines.size()));
    vector<CFotoLine>::iterator it = _lines.begin();
    while (it != _lines.end())
    {
        if ((*it).pnts.size() > 0)
        {
            lines.AppendLine(IntToStr((*it).fid));
            lines.AppendLine(IntToStr((*it).pnts.size()));
            vector<POINT>::const_iterator it_pnts = (*it).pnts.begin();
            while (it_pnts != (*it).pnts.end())
            {
                lines.AppendLine(IntToStr((*it_pnts).x));
                lines.AppendLine(IntToStr((*it_pnts).y));
                it_pnts++;
            }

            if ((*it).pnts.size() == 1)
            {
                lines.AppendLine(IntToStr((*it).pnts[0].x));
                lines.AppendLine(IntToStr((*it).pnts[0].y));
            }

            lines.AppendLine((*it).label);
        }
        it++;
    }

    return String2File(lines.GetText(), fname);
}

bool CFotoSpace::_loadmeasures(char* filename)
{
    string fname;
    if (filename)
    {
        fname = filename;
    }
    else
    {
        fname = RemoveExtNamePart(_filename) + ".fsm";
    }

    return _loadmeasures4thumb(fname, 1, 1);
}

bool CFotoSpace::_generatethumbnail(bool compel)
{
    string dir = Trim(GetDirectoryPart(_filename));
    if (dir == "")
        return false;

    dir = dir + THUMBNAIL_DIR;
    ::CreateDirectory(dir.c_str(), NULL);
    string thumbfile = RemoveDirectoryPart(_filename);
    thumbfile = dir + '\\' + thumbfile;

    if (!compel)
    {
        WIN32_FIND_DATA finddata;
        HANDLE handle = FindFirstFile(thumbfile.c_str(), &finddata);
        if (INVALID_HANDLE_VALUE != handle)
        {
            FindClose(handle);
            return true;
        }
    }

    JPGImage orifoto;
    orifoto.m_mode = 3;
    orifoto.m_nW = _w;
    orifoto.m_nH = _h;
    orifoto.m_pBits = _buff;

    static BYTE thumbbuff[THUMBNAIL_WIDTH*THUMBNAIL_HEIGHT*3];
    JPGImage thumbfoto;
    thumbfoto.m_mode = 3;
    thumbfoto.m_nW = THUMBNAIL_WIDTH;
    thumbfoto.m_nH = THUMBNAIL_HEIGHT;
    thumbfoto.m_pBits = thumbbuff;
    orifoto.ReSample(thumbfoto);
    vertical_reverse_rgb(thumbfoto.m_nW, thumbfoto.m_nH, thumbfoto.m_pBits);
    return thumbfoto.SaveJpg(55, thumbfile.c_str());
}

bool CFotoSpace::_showthumbnail(char* jpgname, bool autogen)
{
    this->_clearall();

    string dir = Trim(GetDirectoryPart(jpgname));
    if (dir == "")
        return false;

    dir = dir + THUMBNAIL_DIR;
    ::CreateDirectory(dir.c_str(), NULL);
    string thumbfile = RemoveDirectoryPart(jpgname);
    thumbfile = dir + '\\' + thumbfile;

    WIN32_FIND_DATA finddata;
    HANDLE handle = FindFirstFile(thumbfile.c_str(), &finddata);
    if (INVALID_HANDLE_VALUE == handle)
    {
        if (autogen)
        {
            return _loadjpg(jpgname, true);
        }
        else
        {
            return false;
        }
    }
    else
    {
        FindClose(handle);
    }

    if (!loadjpg(thumbfile.c_str(), _buff, _w, _h))
    {
        ::memset(_buff, 0, sizeof(_buff));
        return false;
    }

    long test_w, test_h, test_channels;
    if (testjpg(jpgname, test_w, test_h, test_channels))
    {
        string fsmname = RemoveExtNamePart(jpgname) + ".fsm";
        _loadmeasures4thumb(fsmname, double(_w)/test_w, double(_h)/test_h);
    }

    _filename = "";
    return true;
}

bool CFotoSpace::_loadmeasures4thumb(string filename, double ratio_w, double ratio_h)
{
    string txt;
    if (!File2String(filename, txt))
    {
        return false;
    }

    _lines.clear();
    _selectedfid = -1;
    _nextfid = 0;

    Strings lines(txt);
    string line;
    lines.GetLine(0, line);
    long mcount = StrToInt(line);
    long lineindex = 1;
    for (long mindex=0; mindex<mcount; mindex++)
    {
        lines.GetLine(lineindex++, line);
        long fid = StrToInt(line);
        lines.GetLine(lineindex++, line);
        long pntcount = StrToInt(line);
        if (pntcount == 1)
        {
            pntcount = 2;
        }

        long linetype = FOTOMEASURELINE;
        if (pntcount > 2)
        {
            linetype = FOTOMEASURERIGION;
        }

        CFotoLine fotoline(fid, linetype);
        if (fid >= _nextfid)
        {
            _nextfid = fid + 1;
        }

        for (long i=0; i<pntcount; i++)
        {
            POINT pnt;
            lines.GetLine(lineindex++, line);
            pnt.x = StrToInt(line)*ratio_w;
            lines.GetLine(lineindex++, line);
            pnt.y = StrToInt(line)*ratio_h;
            fotoline.pnts.push_back(pnt);
        }

        lines.GetLine(lineindex++, line);
        fotoline.label = line;

        _lines.push_back(fotoline);
    }

    return true;
}