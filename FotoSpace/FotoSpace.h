#if !defined(FOTOSPACE_20130804__INCLUDED_)
#define FOTOSPACE_20130804__INCLUDED_

#include <windows.h>

#pragma warning(disable: 4786)
#include <string>
#include <vector>
using namespace std;

const long FOTOSPACBYTEBUFFSIZE = 99000000;
const long FOTOMEASURELINE = 1;
const long FOTOMEASURERIGION = 2;
const long THUMBNAIL_WIDTH = 400;
const long THUMBNAIL_HEIGHT = 300;

struct CFotoSpace
{
    struct CFotoLine
    {
        long fid;
        long suckit;
        vector<POINT> pnts;
        string label;
        CFotoLine(long newfid, long wth):fid(newfid), suckit(wth){};
    private:
        CFotoLine();
    };

    //��ʼ��
    void _sethwnd(HWND hwnd);
    bool _loadjpg(char* pcfilename, bool genthumb);
    void _clearall();

    //ͼ������
    long _w, _h;//ʵ��pic�ߴ�
    byte _buff[FOTOSPACBYTEBUFFSIZE];

    HWND _hwnd;
    COLORREF _bkgcolor;
    string _filename;


    //���ѡ��ɾ������
    long _addline(long pnt1_x, long pnt1_y, long pnt2_x, long pnt2_y, char* label);
    long _addline(long pnt1_x, long pnt1_y, long pnt2_x, long pnt2_y, double distance);
    void _cleargeometries();
    long _selectbypoint(long pnt_x, long pnt_y);
    bool _selectbyid(long fid);
    long _getselect(long& start_x, long& start_y, long& to_x, long& to_y, char** label);
    void _deleteselection();
    void _deselect();
    long _deletelast(); //ɾ��fid�����Ǹ�
    //---------------------------
    vector<CFotoLine> _lines;
    long _selectedfid;
    long _nextfid;


    //����ת��
    long _screen2pic_x(long x);
    long _screen2pic_y(long y);
    long _pic2screen_x(long x);
    long _pic2screen_y(long y);
    //---------------------------
    POINT _origin;      //�������Ͻ���pic�е�����
    double _scale;      //ͼ�����ű�������widthΪ׼


    //��������
    bool _setscale(double scale);
    void _zoomall();
    void _panstart(long x, long y);
    void _panmoveTo(long x, long y);
    void _panstop();
    //---------------------------
    bool _panstarted;
    POINT _panpntstart;
    POINT _panpntto;

    //�ػ�
    void _updatespace();
    void _drawselection(HDC buffdc, RECT rect, CFotoLine& line);
    void _drawsingleline(CFotoLine line, HDC dc);
    void _drawregion(CFotoLine region, SIZE labelsize, HDC dc);

    //��ʱ����
    void _refreshmeasureline();
    void _drawmeasuresegments(POINT _measurelinestart, POINT _measurelineto);
    void _drawmeasureline(long pnt1_x, long pnt1_y, long pnt2_x, long pnt2_y);
    bool _getmeasureline(long& start_x, long& start_y, long& to_x, long& to_y);
    void _addmeasuresegment(long x, long y);
    bool _getmeasuresegment(long index, long& x, long& y);
    long _getmeasuresegmentcount();
    bool _removelastmeasuresegment();
    long _measure2line(char* label);
    void _clearmeasureline();
    //---------------------------
    vector<POINT> _measuresegments;

    bool _savemeasures(char* filename);
    bool _loadmeasures(char* filename);
    bool _loadmeasures4thumb(string filename, double ratio_w, double ratio_h);

    bool _generatethumbnail(bool compel);
    bool _showthumbnail(char* jpgname, bool autogen);

    CFotoSpace::CFotoSpace();
    CFotoSpace::~CFotoSpace();
};


#endif