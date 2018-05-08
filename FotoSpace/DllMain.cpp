#include "DllMain.h"

#pragma warning(disable: 4786)
#include <vector>
using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

vector<CFotoSpace*> spaces;

long DLL WINAPI newfotospace(long hwnd)
{
    if (!hwnd)
        return -1;

    CFotoSpace* pSpace = new CFotoSpace;
    pSpace->_sethwnd((HWND)hwnd);
    spaces.push_back(pSpace);
    return spaces.size()-1;
}

void DLL WINAPI eraseallfotospaces()
{
    vector<CFotoSpace*>::iterator it = spaces.begin();
    while (it != spaces.end())
    {
        delete *it;
    }

    spaces.clear();
}

bool DLL WINAPI fotospace_loadjpg(long spaceid, char* filename,
    long& img_w, long& img_h, bool loadfsm, bool genthumb)
{
    if (spaces.size() <= spaceid)
        return false;

    if (!spaces[spaceid]->_loadjpg(filename, genthumb))
        return false;

    img_w = spaces[spaceid]->_w;
    img_h = spaces[spaceid]->_h;

    if (loadfsm)
    {
        spaces[spaceid]->_loadmeasures(NULL);
    }

    return true;
}

bool DLL WINAPI fotospace_clear(long spaceid, bool onlygeometries, bool savefsm)
{
    if (spaces.size() <= spaceid)
        return false;

    if (onlygeometries)
    {
        spaces[spaceid]->_cleargeometries();
        if (savefsm)
        {
            spaces[spaceid]->_savemeasures(NULL);
        }
    }
    else
    {
        spaces[spaceid]->_clearall();
    }

    return true;
}

bool DLL WINAPI fotospace_update(long spaceid)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_updatespace();
    return true;
}

long DLL WINAPI fotospace_addline(long spaceid, long pnt1_x, long pnt1_y,
    long pnt2_x, long pnt2_y, char* label, bool savefsm)
{
    if (spaces.size() <= spaceid)
        return -1;

    long r = spaces[spaceid]->_addline(pnt1_x, pnt1_y, pnt2_x, pnt2_y, label);
    if (savefsm)
    {
        spaces[spaceid]->_savemeasures(NULL);
    }

    return r;
}

long DLL WINAPI fotospace_selectbypoint(long spaceid, long pnt_x, long pnt_y)
{
    if (spaces.size() <= spaceid)
        return -1;

    return spaces[spaceid]->_selectbypoint(pnt_x, pnt_y);
}

bool DLL WINAPI fotospace_selectbyid(long spaceid, long fid, long& start_x,
    long& start_y, long& to_x, long& to_y, char** label)
{
    if (spaces.size() <= spaceid)
        return false;

    if (!spaces[spaceid]->_selectbyid(fid))
        return false;

    spaces[spaceid]->_getselect(start_x, start_y, to_x, to_y, label);
    return true;
}

bool DLL WINAPI fotospace_deleteselection(long spaceid, bool savefsm)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_deleteselection();
    if (savefsm)
    {
        spaces[spaceid]->_savemeasures(NULL);
    }

    return true;
}

long DLL WINAPI fotospace_deletelatest(long spaceid, bool savefsm)
{
    if (spaces.size() <= spaceid)
        return -1;

    long fid = spaces[spaceid]->_deletelast();
    if (savefsm)
    {
        spaces[spaceid]->_savemeasures(NULL);
    }

    return fid;
}

bool DLL WINAPI fotospace_zoomall(long spaceid)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_zoomall();
    return true;
}

bool DLL WINAPI fotospace_setscale(long spaceid, double scale)
{
    if (spaces.size() <= spaceid)
        return false;


    return spaces[spaceid]->_setscale(scale);
}

bool DLL WINAPI fotospace_getscale(long spaceid, double& scale)
{
    if (spaces.size() <= spaceid)
        return false;

    scale = spaces[spaceid]->_scale;
    return true;
}

bool DLL WINAPI fotospace_setorigin(long spaceid, long ori_x, long ori_y)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_origin.x = ori_x;
    spaces[spaceid]->_origin.y = ori_y;
    return true;
}

bool DLL WINAPI fotospace_getorigin(long spaceid, long& ori_x, long& ori_y)
{
    if (spaces.size() <= spaceid)
        return false;

    ori_x = spaces[spaceid]->_origin.x;
    ori_y = spaces[spaceid]->_origin.y;
    return true;
}

bool DLL WINAPI fotospace_panstart(long spaceid, long x, long y)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_panstart(x, y);
    return true;
}

bool DLL WINAPI fotospace_panmoveTo(long spaceid, long x, long y)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_panmoveTo(x, y);
    return true;
}

bool DLL WINAPI fotospace_panstop(long spaceid)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_panstop();
    return true;
}

bool DLL WINAPI fotospace_scr2pic(long spaceid, long scr_x, long scr_y, long& pic_x,
    long& pic_y)
{
    if (spaces.size() <= spaceid)
        return false;

    pic_x = spaces[spaceid]->_screen2pic_x(scr_x);
    pic_y = spaces[spaceid]->_screen2pic_y(scr_y);
    return true;
}

bool DLL WINAPI fotospace_pic2scr(long spaceid, long pic_x, long pic_y, long& scr_x,
    long& scr_y)
{
    if (spaces.size() <= spaceid)
        return false;

    scr_x = spaces[spaceid]->_pic2screen_x(pic_x);
    scr_y = spaces[spaceid]->_pic2screen_y(pic_y);
    return true;
}

bool DLL WINAPI fotospace_drawmeasureline(long spaceid, long pnt1_x, long pnt1_y,
    long pnt2_x, long pnt2_y)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_drawmeasureline(pnt1_x, pnt1_y, pnt2_x, pnt2_y);
    return true;
}

bool DLL WINAPI fotospace_getmeasureline(long spaceid, long& start_x, long& start_y,
    long& to_x, long& to_y)
{
    if (spaces.size() <= spaceid)
        return false;

    return spaces[spaceid]->_getmeasureline(start_x, start_y, to_x, to_y);
}

bool DLL WINAPI fotospace_addmeasuresegment(long spaceid, long x, long y)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_addmeasuresegment(x, y);
    return true;
}

bool DLL WINAPI fotospace_getmeasuresegment(long spaceid, long index, long& x, long& y)
{
    if (spaces.size() <= spaceid)
        return false;

    return spaces[spaceid]->_getmeasuresegment(index, x, y);
}

long DLL WINAPI fotospace_getmeasuresegmentcount(long spaceid)
{
    if (spaces.size() <= spaceid)
        return -1;

    return spaces[spaceid]->_getmeasuresegmentcount();
}

bool DLL WINAPI fotospace_removelastmeasuresegment(long spaceid)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_removelastmeasuresegment();
    return true;
}

long DLL WINAPI fotospace_measure2line(long spaceid, char* label, bool savefsm)
{
    if (spaces.size() <= spaceid)
        return -1;

    long r = spaces[spaceid]->_measure2line(label);
    if (savefsm)
    {
        spaces[spaceid]->_savemeasures(NULL);
    }

    return r;
}

bool DLL WINAPI fotospace_clearmeasureline(long spaceid)
{
    if (spaces.size() <= spaceid)
        return false;

    spaces[spaceid]->_clearmeasureline();
    return true;
}

bool DLL WINAPI fotospace_savemeasures(long spaceid, char* filename)
{
    if (spaces.size() <= spaceid)
        return false;

    return spaces[spaceid]->_savemeasures(filename);
}

bool DLL WINAPI fotospace_loadmeasures(long spaceid, char* filename)
{
    if (spaces.size() <= spaceid)
        return false;

    return spaces[spaceid]->_loadmeasures(filename);
}

bool DLL WINAPI fotospace_showthumbnail(long spaceid, char* jpgname, bool autogen)
{
    if (spaces.size() <= spaceid)
        return false;

    return spaces[spaceid]->_showthumbnail(jpgname, autogen);
}