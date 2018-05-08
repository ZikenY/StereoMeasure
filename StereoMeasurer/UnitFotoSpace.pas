unit UnitFotoSpace;

interface

uses
    Windows;

type

Tnewfotospace = function(hwnd: Integer): Integer; stdcall;
Teraseallfotospaces = procedure(); stdcall;
Tfotospace_loadjpg = function(spaceid: Integer; filename: PChar; out img_w: Integer;
    out img_h: Integer; loadfsm: Boolean = False; genthumb: Boolean = False): Boolean; stdcall;
Tfotospace_clear = function(spaceid: Integer; onlygeometries: Boolean;
    savefsm: Boolean = False): Boolean; stdcall;
Tfotospace_update = function(spaceid: Integer): Boolean; stdcall;
Tfotospace_addline = function(spaceid: Integer; pnt1_x: Integer; pnt1_y: Integer;
    pnt2_x: Integer; pnt2_y: Integer; label_: PChar; savefsm: Boolean = False): Integer; stdcall;
Tfotospace_selectbypoint = function(spaceid: Integer; pnt_x: Integer; pnt_y: Integer): Integer; stdcall;
Tfotospace_selectbyid = function(spaceid: Integer; fid: Integer; out start_x:Integer;
    out start_y:Integer; out to_x:Integer; out to_y:Integer; out _label: PChar): Boolean; stdcall;
Tfotospace_deleteselection = function(spaceid: Integer; savefsm: Boolean = False): Boolean; stdcall;
Tfotospace_deletelatest = function(spaceid: Integer; savefsm: Boolean = False): Longint; stdcall;

Tfotospace_zoomall = function(spaceid: Integer): Boolean; stdcall;
Tfotospace_setscale = function(spaceid: Integer; scale: Double): Boolean; stdcall;
Tfotospace_getscale = function(spaceid: Integer; out scale: Double): Boolean; stdcall;
Tfotospace_setorigin = function(spaceid: Integer; ori_x: Integer; ori_y: Integer): Boolean; stdcall;
Tfotospace_getorigin = function(spaceid: Integer; out ori_x: Integer; out ori_y: Integer): Boolean; stdcall;

Tfotospace_panstart = function(spaceid: Integer; x: Integer; y: Integer): Boolean; stdcall;
Tfotospace_panmoveTo = function(spaceid: Integer; x: Integer; y: Integer): Boolean; stdcall;
Tfotospace_panstop = function(spaceid: Integer): Boolean; stdcall;

Tfotospace_scr2pic = function(spaceid: Integer; scr_x: Integer; scr_y: Integer;
    out pic_x: Integer; out pic_y: Integer): Boolean; stdcall;
Tfotospace_pic2scr = function(spaceid: Integer; pic_x: Integer; pic_y: Integer;
    out scr_x: Integer; out scr_y: Integer): Boolean; stdcall;

Tfotospace_drawmeasureline = function(spaceid: Integer; pnt1_x: Integer; pnt1_y: Integer;
    pnt2_x: Integer; pnt2_y: Integer): Boolean; stdcall;
Tfotospace_getmeasureline = function(spaceid: Integer; out start_x: Integer; out start_y: Integer;
    out to_x: Integer; out to_y: Integer): Boolean; stdcall;
Tfotospace_addmeasuresegment = function(spaceid: Integer; x: Longint; y: Longint): Boolean; stdcall;
Tfotospace_getmeasuresegment = function(spaceid: Integer; index: Longint; out x: Longint; out y: Longint): Boolean; stdcall;
Tfotospace_getmeasuresegmentcount = function(spaceid: Integer): Longint; stdcall;
Tfotospace_removelastmeasuresegment = function(spaceid: Integer): Boolean; stdcall;
Tfotospace_measure2line = function(spaceid: Integer; _label: PChar; savefsm: Boolean = False): Integer; stdcall;
Tfotospace_clearmeasureline = function(spaceid: Integer): Boolean; stdcall;

Tfotospace_savemeasures = function(spaceid: Integer; filename: PChar): Boolean; stdcall;
Tfotospace_loadmeasures = function(spaceid: Integer; filename: PChar): Boolean; stdcall;
Tfotospace_showthumbnail = function(spaceid: Integer; jpgname: PChar; autogen: Boolean): Boolean; stdcall;

var
    fotospacemodule: HMODULE;
    newfotospace: Tnewfotospace;
    eraseallfotospaces: Teraseallfotospaces;
    fotospace_loadjpg: Tfotospace_loadjpg;
    fotospace_clear: Tfotospace_clear;
    fotospace_update: Tfotospace_update;
    fotospace_addline: Tfotospace_addline;
    fotospace_selectbypoint: Tfotospace_selectbypoint;
    fotospace_selectbyid: Tfotospace_selectbyid;
    fotospace_deleteselection: Tfotospace_deleteselection;
    fotospace_deletelatest: Tfotospace_deletelatest;
    fotospace_zoomall: Tfotospace_zoomall;
    fotospace_setscale: Tfotospace_setscale;
    fotospace_getscale: Tfotospace_getscale;
    fotospace_setorigin: Tfotospace_setorigin;
    fotospace_getorigin: Tfotospace_getorigin;
    fotospace_panstart: Tfotospace_panstart;
    fotospace_panmoveTo: Tfotospace_panmoveTo;
    fotospace_panstop: Tfotospace_panstop;
    fotospace_scr2pic: Tfotospace_scr2pic;
    fotospace_pic2scr: Tfotospace_pic2scr;
    fotospace_drawmeasureline: Tfotospace_drawmeasureline;
    fotospace_getmeasureline: Tfotospace_getmeasureline;
    fotospace_addmeasuresegment: Tfotospace_addmeasuresegment;
    fotospace_getmeasuresegment: Tfotospace_getmeasuresegment;
    fotospace_getmeasuresegmentcount: Tfotospace_getmeasuresegmentcount;
    fotospace_removelastmeasuresegment: Tfotospace_removelastmeasuresegment;
    fotospace_measure2line: Tfotospace_measure2line;
    fotospace_clearmeasureline: Tfotospace_clearmeasureline;
    fotospace_savemeasures: Tfotospace_savemeasures;
    fotospace_loadmeasures: Tfotospace_loadmeasures;
    fotospace_showthumbnail: Tfotospace_showthumbnail;

implementation

function InitialFotoSpaceLib(const filename: PChar): Boolean;
begin
    Result := False;
    fotospacemodule := LoadLibrary(filename);
    if (0 = fotospacemodule) then begin
        Exit;
    end;

    newfotospace := Tnewfotospace(GetProcAddress(fotospacemodule, 'newfotospace'));
    eraseallfotospaces := Teraseallfotospaces(GetProcAddress(fotospacemodule, 'eraseallfotospaces'));
    fotospace_loadjpg := Tfotospace_loadjpg(GetProcAddress(fotospacemodule, 'fotospace_loadjpg'));
    fotospace_clear := Tfotospace_clear(GetProcAddress(fotospacemodule, 'fotospace_clear'));
    fotospace_update := Tfotospace_update(GetProcAddress(fotospacemodule, 'fotospace_update'));
    fotospace_zoomall := Tfotospace_zoomall(GetProcAddress(fotospacemodule, 'fotospace_zoomall'));
    fotospace_addline := Tfotospace_addline(GetProcAddress(fotospacemodule, 'fotospace_addline'));
    fotospace_selectbypoint := Tfotospace_selectbypoint(GetProcAddress(fotospacemodule, 'fotospace_selectbypoint'));
    fotospace_selectbyid := Tfotospace_selectbyid(GetProcAddress(fotospacemodule, 'fotospace_selectbyid'));
    fotospace_deleteselection := Tfotospace_deleteselection(GetProcAddress(fotospacemodule, 'fotospace_deleteselection'));
    fotospace_deletelatest := Tfotospace_deletelatest(GetProcAddress(fotospacemodule, 'fotospace_deletelatest'));
    fotospace_setscale := Tfotospace_setscale(GetProcAddress(fotospacemodule, 'fotospace_setscale'));
    fotospace_getscale := Tfotospace_getscale(GetProcAddress(fotospacemodule, 'fotospace_getscale'));
    fotospace_setorigin := Tfotospace_setorigin(GetProcAddress(fotospacemodule, 'fotospace_setorigin'));
    fotospace_getorigin := Tfotospace_getorigin(GetProcAddress(fotospacemodule, 'fotospace_getorigin'));
    fotospace_panstart := Tfotospace_panstart(GetProcAddress(fotospacemodule, 'fotospace_panstart'));
    fotospace_panmoveTo := Tfotospace_panmoveTo(GetProcAddress(fotospacemodule, 'fotospace_panmoveTo'));
    fotospace_panstop := Tfotospace_panstop(GetProcAddress(fotospacemodule, 'fotospace_panstop'));
    fotospace_scr2pic := Tfotospace_scr2pic(GetProcAddress(fotospacemodule, 'fotospace_scr2pic'));
    fotospace_pic2scr := Tfotospace_pic2scr(GetProcAddress(fotospacemodule, 'fotospace_pic2scr'));
    fotospace_drawmeasureline := Tfotospace_drawmeasureline(GetProcAddress(fotospacemodule, 'fotospace_drawmeasureline'));
    fotospace_getmeasureline := Tfotospace_getmeasureline(GetProcAddress(fotospacemodule, 'fotospace_getmeasureline'));
    fotospace_addmeasuresegment := Tfotospace_addmeasuresegment(GetProcAddress(fotospacemodule, 'fotospace_addmeasuresegment'));
    fotospace_getmeasuresegment := Tfotospace_getmeasuresegment(GetProcAddress(fotospacemodule, 'fotospace_getmeasuresegment'));
    fotospace_getmeasuresegmentcount := Tfotospace_getmeasuresegmentcount(GetProcAddress(fotospacemodule, 'fotospace_getmeasuresegmentcount'));
    fotospace_removelastmeasuresegment := Tfotospace_removelastmeasuresegment(GetProcAddress(fotospacemodule, 'fotospace_removelastmeasuresegment'));
    fotospace_measure2line := Tfotospace_measure2line(GetProcAddress(fotospacemodule, 'fotospace_measure2line'));
    fotospace_clearmeasureline := Tfotospace_clearmeasureline(GetProcAddress(fotospacemodule, 'fotospace_clearmeasureline'));
    fotospace_savemeasures := Tfotospace_savemeasures(GetProcAddress(fotospacemodule, 'fotospace_savemeasures'));
    fotospace_loadmeasures := Tfotospace_loadmeasures(GetProcAddress(fotospacemodule, 'fotospace_loadmeasures'));
    fotospace_showthumbnail := Tfotospace_showthumbnail(GetProcAddress(fotospacemodule, 'fotospace_showthumbnail'));
    Result := True;
end;

////////////////////////////////////////////////////////////////////////////////
initialization
    InitialFotoSpaceLib('FotoSpace.dll');

finalization
    if (0 <> fotospacemodule) then begin
        FreeLibrary(fotospacemodule);
    end;

end.
