#if !defined(DLLMAIN_20130806__INCLUDED_)
#define DLLMAIN_20130806__INCLUDED_

#include "FotoSpace.h"

#if !defined(WINAPI)
    #define WINAPI __stdcall
#endif

#if !defined(DLL)
    #define DLL __declspec(dllexport)
#endif

long DLL WINAPI newfotospace(long hwnd);
void DLL WINAPI eraseallfotospaces();

bool DLL WINAPI fotospace_loadjpg(long spaceid, char* filename, long& img_w, long& img_h,
    bool loadfsm, bool genthumb);
bool DLL WINAPI fotospace_clear(long spaceid, bool onlygeometries, bool savefsm);
bool DLL WINAPI fotospace_update(long spaceid);
long DLL WINAPI fotospace_addline(long spaceid, long pnt1_x, long pnt1_y,
    long pnt2_x, long pnt2_y, char* label, bool savefsm);
long DLL WINAPI fotospace_selectbypoint(long spaceid, long pnt_x, long pnt_y);
bool DLL WINAPI fotospace_selectbyid(long spaceid, long fid, long& start_x,
    long& start_y, long& to_x, long& to_y, char** label);
bool DLL WINAPI fotospace_deleteselection(long spaceid, bool savefsm);
long DLL WINAPI fotospace_deletelatest(long spaceid, bool savefsm);

bool DLL WINAPI fotospace_zoomall(long spaceid);
bool DLL WINAPI fotospace_setscale(long spaceid, double scale);
bool DLL WINAPI fotospace_getscale(long spaceid, double& scale);
bool DLL WINAPI fotospace_setorigin(long spaceid, long ori_x, long ori_y);
bool DLL WINAPI fotospace_getorigin(long spaceid, long& ori_x, long& ori_y);
bool DLL WINAPI fotospace_panstart(long spaceid, long x, long y);
bool DLL WINAPI fotospace_panmoveTo(long spaceid, long x, long y);
bool DLL WINAPI fotospace_panstop(long spaceid);

bool DLL WINAPI fotospace_scr2pic(long spaceid, long scr_x, long scr_y, long& pic_x,
    long& pic_y);
bool DLL WINAPI fotospace_pic2scr(long spaceid, long pic_x, long pic_y, long& scr_x,
    long& scr_y);

bool DLL WINAPI fotospace_drawmeasureline(long spaceid, long pnt1_x, long pnt1_y,
    long pnt2_x, long pnt2_y);
bool DLL WINAPI fotospace_getmeasureline(long spaceid, long& start_x, long& start_y,
    long& to_x, long& to_y);
bool DLL WINAPI fotospace_addmeasuresegment(long spaceid, long x, long y);
bool DLL WINAPI fotospace_getmeasuresegment(long spaceid, long index, long& x, long& y);
long DLL WINAPI fotospace_getmeasuresegmentcount(long spaceid);
bool DLL WINAPI fotospace_removelastmeasuresegment(long spaceid);
long DLL WINAPI fotospace_measure2line(long spaceid, char* label, bool savefsm);
bool DLL WINAPI fotospace_clearmeasureline(long spaceid);

bool DLL WINAPI fotospace_savemeasures(long spaceid, char* filename);
bool DLL WINAPI fotospace_loadmeasures(long spaceid, char* filename);
bool DLL WINAPI fotospace_showthumbnail(long spaceid, char* jpgname, bool autogen);

#endif