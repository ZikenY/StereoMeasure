unit UnitMainFrm;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, ToolWin, ImgList, Buttons, UnitPlacemark,
  UnitStereoMeasure, UnitFotoSpace, UnitCalibration1, UnitCalibration2,
  Menus, jpeg;

const MOUSECURSOR_DEFAULT       = 1;
const MOUSECURSOR_ARROW1        = MOUSECURSOR_DEFAULT + 1;
const MOUSECURSOR_ARROW2        = MOUSECURSOR_ARROW1 + 1;
const MOUSECURSOR_CROSS1        = MOUSECURSOR_ARROW2 + 1;
const MOUSECURSOR_CROSS2        = MOUSECURSOR_CROSS1 + 1;
const MOUSECURSOR_CROSSBOX      = MOUSECURSOR_CROSS2 + 1;
const MOUSECURSOR_PICKUP        = MOUSECURSOR_CROSSBOX + 1;
const MOUSECURSOR_MOVE          = MOUSECURSOR_PICKUP + 1;
const MOUSECURSOR_COPY          = MOUSECURSOR_MOVE + 1;
const MOUSECURSOR_DRAG          = MOUSECURSOR_COPY + 1;
const MOUSECURSOR_BEAM          = MOUSECURSOR_DRAG + 1;
const MOUSECURSOR_CANCEL        = MOUSECURSOR_BEAM + 1;
const MOUSECURSOR_MEASURE       = MOUSECURSOR_CANCEL + 1;
const MOUSECURSOR_HAND          = MOUSECURSOR_MEASURE + 1;
const MOUSECURSOR_ZOOM          = MOUSECURSOR_HAND + 1;
const MOUSECURSOR_ZOOMIN        = MOUSECURSOR_ZOOM + 1;
const MOUSECURSOR_ZOOMOUT       = MOUSECURSOR_ZOOMIN + 1;
const MOUSECURSOR_BUSYWAIT      = MOUSECURSOR_ZOOMOUT + 1;
const MOUSECURSOR_BARRIER       = MOUSECURSOR_BUSYWAIT + 1;
const MOUSECURSOR_FOX           = MOUSECURSOR_BARRIER + 1;
const MOUSECURSOR_PIGGY         = MOUSECURSOR_FOX + 1;
const MOUSECURSOR_HOLD          = MOUSECURSOR_PIGGY + 1;
const MOUSECURSOR_CATCH         = MOUSECURSOR_HOLD + 1;
const MOUSECURSOR_PENCIL        = MOUSECURSOR_CATCH + 1;
const MOUSECURSOR_BULLSEYE      = MOUSECURSOR_PENCIL + 1;
const MOUSECURSOR_TERMINATOR    = MOUSECURSOR_BULLSEYE + 1;
const MOUSECURSOR_PALETTE       = MOUSECURSOR_TERMINATOR + 1;

type
    TPIntArr100 = ^TIntArr100;
    TIntArr100 = array [0..99] of Integer;

type
    TMeasureLineScr = record
        p1: tagPOINT;
        p2: tagPOINT;
        distance: Double;
    end;

type
  TFormMain = class(TForm)
    OpenDialogJpg: TOpenDialog;
    TimerDelayRedraw: TTimer;
    PanelTop: TPanel;
    PanelBaseL: TPanel;
    PanelBaseR: TPanel;
    PanelL: TPanel;
    PanelR: TPanel;
    Splitter1: TSplitter;
    PanelBottom: TPanel;
    SplitterBase: TSplitter;
    CBBindZoom: TCheckBox;
    TimerZommAll: TTimer;
    OpenDialogPar: TOpenDialog;
    OpenDialogChessboards: TOpenDialog;
    SpeedButton1: TSpeedButton;
    SBLoadStereo: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    SpeedButton8: TSpeedButton;
    SpeedButton9: TSpeedButton;
    SpeedButton10: TSpeedButton;
    SpeedButton11: TSpeedButton;
    PanelLeftJpg: TPanel;
    PanelRightJpg: TPanel;
    PanelBottomPar: TPanel;
    PanelBottomRightSide: TPanel;
    TimerRectify: TTimer;
    TimerPreviousFoto: TTimer;
    TimerNextFoto: TTimer;
    CBRectify: TCheckBox;
    SBMatch: TSpeedButton;
    SBUnCancelTrack: TSpeedButton;
    CBAccurate: TCheckBox;
    SBMZoom: TSpeedButton;
    SBMBrowse: TSpeedButton;
    PopupMenuCalibrate: TPopupMenu;
    CalibrateSeparate1: TMenuItem;
    CalibrateMixedFotos1: TMenuItem;
    SBCalibrate: TSpeedButton;
    PanelWait: TPanel;
    Image1: TImage;
    SpeedButton12: TSpeedButton;
    TimerRemoveLast2Left: TTimer;
    TimerRemoveLast2Right: TTimer;
    PanelScroll: TPanel;
    TrackBarAllFotos: TTrackBar;
    SBPreviousFoto: TSpeedButton;
    SBNextFoto: TSpeedButton;
    PopupMenuRecentList: TPopupMenu;
    procedure FormShow(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure TimerDelayRedrawTimer(Sender: TObject);
    procedure PanelLMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PanelRMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormResize(Sender: TObject);
    procedure PanelLMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PanelRMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PanelLMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PanelRMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure FormMouseWheelDown(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure FormMouseWheelUp(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure TimerZommAllTimer(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SBLoadStereoClick(Sender: TObject);
    procedure SpeedButton11Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton7Click(Sender: TObject);
    procedure SpeedButton10Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure SpeedButton9Click(Sender: TObject);
    procedure SpeedButton8Click(Sender: TObject);
    procedure SBPreviousFotoClick(Sender: TObject);
    procedure TimerRectifyTimer(Sender: TObject);
    procedure SBNextFotoClick(Sender: TObject);
    procedure SBMZoomClick(Sender: TObject);
    procedure SBMBrowseClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure TimerPreviousFotoTimer(Sender: TObject);
    procedure TimerNextFotoTimer(Sender: TObject);
    procedure CBRectifyClick(Sender: TObject);
    procedure SBMatchClick(Sender: TObject);
    procedure SBUnCancelTrackClick(Sender: TObject);
    procedure SBCalibrateClick(Sender: TObject);
    procedure CalibrateSeparate1Click(Sender: TObject);
    procedure CalibrateMixedFotos1Click(Sender: TObject);
    procedure SpeedButton12Click(Sender: TObject);
    procedure PanelLDblClick(Sender: TObject);
    procedure TimerRemoveLast2LeftTimer(Sender: TObject);
    procedure PanelRDblClick(Sender: TObject);
    procedure TimerRemoveLast2RightTimer(Sender: TObject);
    procedure TrackBarAllFotosEnter(Sender: TObject);
    procedure TrackBarAllFotosChange(Sender: TObject);
    procedure CBBindZoomClick(Sender: TObject);
    procedure CBAccurateClick(Sender: TObject);
  private
    { Private declarations }

  public

    imagewidth, imageheight: Integer;
    mousepoints_x_l: TIntArr100;
    mousepoints_y_l: TIntArr100;
    mousepoints_x_r: TIntArr100;
    mousepoints_y_r: TIntArr100;
    mouseleftcount_l: Integer;
    mouseleftcount_r: Integer;
    regionpnt_l: Integer;
    regionpnt_r: Integer;
    reselectregion_l: Boolean;
    reselectregion_r: Boolean;

    left_panstarted: Boolean;
    right_panstarted: Boolean;
    fotos_l, fotos_r: TStringList;//列出可能的前后图片

    parfilename: AnsiString;
    fotosbasedir: AnsiString;
    middlepath: AnsiString;
    leftjpg: AnsiString;
    rightjpg: AnsiString;
    leftfsm: AnsiString;
    rightfsm: AnsiString;
    realleftjpg: AnsiString;
    realrightjpg: AnsiString;
    matchhomodirty: Boolean;

    function calc_distance(out d: Double): Boolean;
    function checkdistance(): Boolean;
    function placemark(spaceid: Longint; x: Longint; y: Longint): Boolean;
    procedure SaveMeasure2Lines();
    function CalcArea(pointcount: Longint): Double;
    procedure bindrightpanel(origin: Integer);
    procedure loadthumbnails(ljpg: AnsiString; rjpg: AnsiString);
    procedure loadunrectifystereo(ljpg: AnsiString; rjpg: AnsiString);
    function GetRightFoto(leftfoto: AnsiString): AnsiString;
    function GetNextFotoNames(out nextleft: AnsiString;
        out nextright: AnsiString; Previous: Boolean = False): Boolean;
    procedure LoadMatchFotos();
    procedure ResetTraceStatus();
    procedure LoadAllFoto();
    procedure AddLoadLeftName(leftname: AnsiString);
    procedure RecentItemClick(Sender: TObject);
    procedure LoadLeftFoto(leftfoto: AnsiString);
  end;

var
    FormMain: TFormMain;
    frmplacemark: TFormPlacemark;
    frmCalibration1: TFormCalibration1;
    frmCalibration2: TFormCalibration2;

implementation

{$R *.DFM}
{$R cursors.res}


function StereoCalibcallback(progress: Longint; msg: PChar; fotofilename: PChar;
    isleft: Boolean): Boolean; stdcall;
var
    w, h: Longint;
begin
    if (msg = 'cvStereoCalibrate()...') then begin
        FormMain.PanelWait.Visible := True;
    end
    else begin
        FormMain.PanelWait.Visible := False;
    end;

    if (fotofilename <> nil) then begin
        if (isleft) then begin
            fotospace_loadjpg(0, fotofilename, w, h);
            fotospace_zoomall(0);
            fotospace_update(0);
        end
        else begin
            fotospace_loadjpg(1, fotofilename, w, h);
            fotospace_zoomall(1);
            fotospace_update(1);
        end;
    end;

    Application.ProcessMessages();
    FormMain.PanelBottomPar.Caption := msg + '  ' + IntToStr(progress) + '%';
    FormMain.Refresh();
    Result := True;
end;

function TFormMain.GetNextFotoNames(out nextleft: AnsiString;
    out nextright: AnsiString; Previous: Boolean): Boolean;
var
    tmp, fotol, fotor, dirl, dirr: AnsiString;
    suffix: AnsiString;
    must, n, dashpos: Longint;
    fd: TWin32FindData;
    handle: THandle;
begin
    Result := False;
    nextleft := '';
    nextright := '';

    tmp := LowerCase(ChangeFileExt(ExtractFileName(leftjpg), ''));
    suffix := Copy(tmp, Length(tmp), 1);
    if (suffix = 'l') then begin
        tmp := Copy(tmp, 1, Length(tmp)-1);
        try
            n := StrToInt(tmp);
            must := Length(tmp);

            if (Previous) then begin
                Dec(n);
            end
            else begin
                Inc(n);
            end;
            tmp := IntToStr(n);
            while (must > Length(tmp)) do begin
                tmp := '0' + tmp;
            end;

            fotol := ExtractFilePath(leftjpg) + tmp + 'l.jpg';
            fotor := ExtractFilePath(leftjpg) + tmp + 'r.jpg';
            handle := windows.FindFirstFile(PChar(fotol), fd);
            if (handle = INVALID_HANDLE_VALUE) then begin
                Exit;
            end;
            windows.FindClose(handle);
            handle := windows.FindFirstFile(PChar(fotor), fd);
            if (handle = INVALID_HANDLE_VALUE) then begin
                Exit;
            end;
            windows.FindClose(handle);
            nextleft := fotol;
            nextright := fotor;
            Result := True;
        except
        end;
        Exit;
    end;

    tmp := ExtractFileName(ChangeFileExt(leftjpg, ''));
    try
        n := StrToInt(tmp);
        must := Length(tmp);
    except
        dashpos := Pos('-', tmp);
        if (dashpos = 0) then begin
            dashpos := Pos('_', tmp);
            if (dashpos = 0) then begin
                Exit;
            end;
        end;

        tmp := Copy(tmp, 0, dashpos-1);
        try
            n := StrToInt(tmp);
            must := Length(tmp);
        except
            /////////
            Exit;
        end;
    end;

    if (Previous) then begin
        Dec(n);
    end
    else begin
        Inc(n);
    end;
    tmp := IntToStr(n);
    while (must > Length(tmp)) do begin
        tmp := '0' + tmp;
    end;

    if (middlepath = '') then begin
        dirl := fotosbasedir + '\Left\';
        dirr := fotosbasedir + '\Right\';
    end
    else begin
        dirl := fotosbasedir + '\Left\' + middlepath + '\';
        dirr := fotosbasedir + '\Right\' + middlepath + '\';
    end;
    fotol := dirl + tmp + '*.jpg';
    fotor := dirr + tmp + '*.jpg';

    handle := windows.FindFirstFile(PChar(fotol), fd);
    nextleft := dirl + fd.cFileName;
    if (handle = INVALID_HANDLE_VALUE) then begin
        Exit;
    end;
    windows.FindClose(handle);

    handle := windows.FindFirstFile(PChar(fotor), fd);
    nextright := dirr + fd.cFileName;
    if (handle = INVALID_HANDLE_VALUE) then begin
        Exit;
    end;
    windows.FindClose(handle);

    Result := True;
end;

function TFormMain.GetRightFoto(leftfoto: AnsiString): AnsiString;
var
    dir: AnsiString;
    tmp: AnsiString;
    leftname: AnsiString;
    viablename: AnsiString;
    dashpos: Longint;
    rightdir, rightfoto: AnsiString;
    fd: TWin32FindData;
    handle: THandle;
    suffix: AnsiString;
begin
    Result := '';
    fotosbasedir := '';
    middlepath  := '';

    tmp := LowerCase(ChangeFileExt(ExtractFileName(leftfoto), ''));
    suffix := Copy(tmp, Length(tmp), 1);
    if (suffix = 'l') then begin
        //同级目录下
        tmp := ChangeFileExt(leftfoto, '');
        tmp := Copy(tmp, 1, Length(tmp)-1) + 'r.jpg';

        handle := windows.FindFirstFile(PChar(tmp), fd);
        if (handle <> INVALID_HANDLE_VALUE) then begin
            windows.FindClose(handle);
            Result := tmp;
            fotosbasedir := ExtractFilePath(leftfoto);
            Delete(fotosbasedir, Length(fotosbasedir), 1);
            middlepath  := '';
        end;
        Exit;
    end;


    dir := ExtractFilePath(leftfoto);
    Delete(dir, Length(dir), 1);
    tmp := LowerCase(ExtractFileName(dir));
    if (tmp = 'left') then begin
        fotosbasedir := ExtractFilePath(dir);
        Delete(fotosbasedir, Length(fotosbasedir), 1);
    end
    else begin
        middlepath := tmp;
        dir := ExtractFilePath(dir);
        Delete(dir, Length(dir), 1);
        tmp := LowerCase(ExtractFileName(dir));
        if (tmp = 'left') then begin
            fotosbasedir := ExtractFilePath(dir);
            Delete(fotosbasedir, Length(fotosbasedir), 1);
        end
        else begin
            middlepath := '';
            Exit;
        end;
    end;

    leftname := ExtractFileName(leftfoto);
    if (middlepath = '') then begin
        rightdir := fotosbasedir + '\Right\';
    end
    else begin
        rightdir := fotosbasedir + '\Right\' + middlepath + '\';
    end;
    rightfoto := rightdir + leftname;

    handle := windows.FindFirstFile(PChar(rightfoto), fd);
    if (handle <> INVALID_HANDLE_VALUE) then begin
        Result := rightfoto;
        windows.FindClose(handle);
        Exit;
    end;

    viablename := ChangeFileExt(leftname, '');
    dashpos := Pos('-', viablename);
    if (dashpos = 0) then begin
        dashpos := Pos('_', viablename);
        if (dashpos = 0) then begin
            Result := '';
            fotosbasedir := '';
            middlepath  := '';
            Exit;
        end;
    end;

    viablename := Copy(viablename, 0, dashpos-1);

    handle := windows.FindFirstFile(PChar(AnsiString(rightdir + viablename + '*.jpg')), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        Result := '';
        fotosbasedir := '';
        middlepath  := '';
        Exit;
    end;

    Result := rightdir + fd.cFileName;
    windows.FindClose(handle);
end;

procedure TFormMain.loadthumbnails(ljpg: AnsiString; rjpg: AnsiString);
begin
    PanelLeftJpg.Caption := ExtractFileName(ljpg);
    PanelRightJpg.Caption := ExtractFileName(rjpg);

    leftfsm := '';
    rightfsm := '';
    fotospace_showthumbnail(0, PChar(ljpg), True); 
    fotospace_showthumbnail(1, PChar(rjpg), True);
end;

procedure TFormMain.loadunrectifystereo(ljpg: AnsiString; rjpg: AnsiString);
var
    temppath: array [0..MAX_PATH] of Char;
    tmpjpg_l, tmpjpg_r: AnsiString;
begin
    Self.leftjpg := ljpg;   
    Self.rightjpg := rjpg;
    PanelLeftJpg.Caption := ExtractFileName(ljpg);
    PanelRightJpg.Caption := ExtractFileName(rjpg);

    GetTempPath(MAX_PATH, temppath);
    tmpjpg_l := AnsiString(temppath) + 'oops_l.jpg';
    tmpjpg_r := AnsiString(temppath) + 'oops_r.jpg';

    if (CBRectify.Checked) then begin
        UndistortedImage(PChar(leftjpg), PChar(tmpjpg_l), True);
        UndistortedImage(PChar(rightjpg), PChar(tmpjpg_r), True);
    end
    else begin
        tmpjpg_l := leftjpg;
        tmpjpg_r := rightjpg;
    end;

    leftfsm := '';
    rightfsm := '';
    if (fotospace_loadjpg(0, PChar(tmpjpg_l), imagewidth, imageheight)) then begin
        leftfsm := ChangeFileExt(leftjpg, '.fsm');
        fotospace_loadmeasures(0, PChar(leftfsm));
        realleftjpg := tmpjpg_l;
    end;

    if (fotospace_loadjpg(1, PChar(tmpjpg_r), imagewidth, imageheight)) then begin
        rightfsm := ChangeFileExt(rightjpg, '.fsm');
        fotospace_loadmeasures(1, PChar(rightfsm));
        realrightjpg := tmpjpg_r;
    end;

    matchhomodirty := True;
end;

procedure TFormMain.bindrightpanel(origin: Integer);
var
    x, y: Integer;
    scale: Double;
begin
    if (not CBBindZoom.Checked) then begin
        Exit;
    end;

    if (origin = 0) then begin
        fotospace_getscale(0, scale);
        fotospace_setscale(1, scale);
        fotospace_getorigin(0, x, y);
        fotospace_setorigin(1, x, y);
        fotospace_update(1);
    end
    else begin
        fotospace_getscale(1, scale);
        fotospace_setscale(0, scale);
        fotospace_getorigin(1, x, y);
        fotospace_setorigin(0, x, y);
        fotospace_update(0);
    end;
end;

function TFormMain.calc_distance(out d: Double): Boolean;
var
    left_pt1: TIntArr100;
    right_pt1: TIntArr100;
    left_pt2: TIntArr100;
    right_pt2: TIntArr100;
begin
    Result := False;
    if ((mouseleftcount_l < 2) or (mouseleftcount_r < 2)) then begin
        Exit;
    end;

    fotospace_getmeasureline(0, left_pt1[0], left_pt1[1], left_pt2[0], left_pt2[1]);
    fotospace_getmeasureline(1, right_pt1[0], right_pt1[1], right_pt2[0], right_pt2[1]);

    d := MeasureDistance1(@left_pt1[0], @right_pt1[0], @left_pt2[0], @right_pt2[0]);

    Result := True;
end;

procedure TFormMain.SaveMeasure2Lines();
var
    txt: AnsiString;
    x1, y1, x2, y2: Longint;
    x1r, y1r, x2r, y2r: Longint;
    dist: Double;
    segcount: Longint;
begin
    fotospace_getmeasureline(0, x1, y1, x2, y2);
    fotospace_getmeasureline(1, x1r, y1r, x2r, y2r);

    if (PanelL.Tag = 3) then begin

        if (frmplacemark.CBGetDistance.Checked) then begin
            dist := MeasurePosition(x1, y1, x1r, y1r) / 100.0;
            txt := AnsiString(frmplacemark.Edit1.Text) + ', far: ' + Format('%n米',[dist]);
        end
        else begin
            txt := AnsiString(frmplacemark.Edit1.Text);
        end;

        fotospace_measure2line(0, PChar(txt));
        fotospace_measure2line(1, PChar(txt));

        fotospace_savemeasures(0, PChar(leftfsm));
        fotospace_savemeasures(1, PChar(rightfsm));

        mouseleftcount_l := 0;
        mouseleftcount_r := 0;

        fotospace_update(0);
        fotospace_update(1);
    end
    else if (PanelL.Tag = 0) then begin
        if ((mouseleftcount_l >= 2) or (mouseleftcount_r >= 2)) then begin
            mouseleftcount_l := 2;
            mouseleftcount_r := 2;
        end;
        checkdistance();
    end
    else if (PanelL.Tag = 4) then begin
        segcount := fotospace_getmeasuresegmentcount(0);
        if (segcount < 3) then begin
            ShowMessage('the minimal point count is 3.');
        end
        else if (segcount <> fotospace_getmeasuresegmentcount(1)) then begin
            ShowMessage('the left & right point counts must be identical.');
        end
        else begin
            dist := CalcArea(segcount)  / 10000;
//            txt := 'area: ' + FloatToStr(dist);
            txt := Format('%nm^2',[dist]);
            fotospace_measure2line(0, PChar(txt));
            fotospace_measure2line(1, PChar(txt));
            fotospace_savemeasures(0, PChar(leftfsm));
            fotospace_savemeasures(1, PChar(rightfsm));
        end;
        
        mouseleftcount_l := 0;
        mouseleftcount_r := 0;
        regionpnt_l := 0;
        regionpnt_r := 0;
        reselectregion_l := False;
        reselectregion_l := False;
        fotospace_update(0);
        fotospace_update(1);
    end;

    fotospace_clearmeasureline(0);
    fotospace_clearmeasureline(1);
end;

function TFormMain.CalcArea(pointcount: Longint): Double;
var
    left_pt1: TIntArr100;
    right_pt1: TIntArr100;
    left_pt2: TIntArr100;
    right_pt2: TIntArr100;
    left_pt3: TIntArr100;
    right_pt3: TIntArr100;
    area: Double;
    i: Longint;
begin
    area := 0;
    for i:=1 to pointcount-2 do begin
        fotospace_getmeasuresegment(0, 0, left_pt1[0], left_pt1[1]);
        fotospace_getmeasuresegment(1, 0, right_pt1[0], right_pt1[1]);

        fotospace_getmeasuresegment(0, i, left_pt2[0], left_pt2[1]);
        fotospace_getmeasuresegment(1, i, right_pt2[0], right_pt2[1]);

        fotospace_getmeasuresegment(0, i+1, left_pt3[0], left_pt3[1]);
        fotospace_getmeasuresegment(1, i+1, right_pt3[0], right_pt3[1]);

        area := area + MeasureAreaTriangle1(@left_pt1[0], @right_pt1[0], @left_pt2[0],
          @right_pt2[0], @left_pt3[0], @right_pt3[0]);
    end;

    Result := area;
end;

function TFormMain.placemark(spaceid: Longint; x: Longint; y: Longint): Boolean;
begin
    Result := False;
    fotospace_drawmeasureline(spaceid, x, y, x, y);
    if (mouseleftcount_l = 0) or (mouseleftcount_r = 0) then begin
        Exit;
    end;

    SaveMeasure2Lines();
    Result := True;
end;

function TFormMain.checkdistance(): Boolean;
var
    d: Double;
begin
    Result := False;
    if (calc_distance(d)) then begin
        d := d / 100;

        fotospace_measure2line(0, PChar(Format('%n米',[d])));
        fotospace_measure2line(1, PChar(Format('%n米',[d])));

        fotospace_savemeasures(0, PChar(leftfsm));
        fotospace_savemeasures(1, PChar(rightfsm));

        mouseleftcount_l := 0;
        mouseleftcount_r := 0;

        fotospace_update(0);
        fotospace_update(1);
        Result:= True;
    end;
end;

procedure TFormMain.FormShow(Sender: TObject);
var
    fd: TWin32FindData;
    handle: THandle;
    strlist: TStringList;
    dir, tmpleft, tmpright: AnsiString;
    i, count, nextline: Longint;
    recentname: AnsiString;
begin
    if (0 = stereomeasuremodule) then begin
        MessageBox(0, 'no StereoMeasure.dll', '', MB_OK);
        Application.Terminate();
    end;                  

    if (0 = fotospacemodule) then begin
        MessageBox(0, 'no FotoSpace.dll', '', MB_OK);
        Application.Terminate();
    end;

    SBMBrowse.Left := SBMZoom.Left;
    SBMBrowse.Top := SBMZoom.Top;
    PanelBottomRightSide.Align := alRight;
    PanelBottomPar.Align := alLeft;
    PanelLeftJpg.Align := alBottom;
    PanelRightJpg.Align := alBottom;
    PanelTop.Align := alTop;
    PanelL.Align := alClient;
    PanelR.Align := alClient;
    PanelBottom.Align := alBottom;
    SplitterBase.Align := alBottom;
    PanelBaseL.Align := alLeft;
    Splitter1.Align := alLeft;
    PanelBaseR.Align := alClient;
    PanelBaseL.Width := Self.Width div 2 - 10;
    PanelScroll.Align := alClient;

    newfotospace(PanelL.Handle);
    newfotospace(PanelR.Handle);

    strlist := TStringList.Create();
    try
        strlist.LoadFromFile(ExtractFilePath(Application.ExeName)
            + ChangeFileExt(ExtractFileName(Application.ExeName), '.cfg'));
        nextline := 0;
        parfilename := strlist.Strings[nextline];
        Inc(nextline);
        leftjpg := strlist.Strings[nextline];
        Inc(nextline);
        rightjpg := strlist.Strings[nextline];
        Inc(nextline);
        FrmPlacemark.Edit1.Text := strlist.Strings[nextline];
        Inc(nextline);
        count := StrToInt(strlist.Strings[nextline]);
        Inc(nextline);
        for i:=0 to count-1 do begin
            recentname := strlist.Strings[nextline];
            Inc(nextline);
            AddLoadLeftName(recentname);
        end;
    except
    end;
    strlist.Free();

    GetRightFoto(leftjpg);

    PanelBottomPar.Caption := '<no par>';
    dir := ExtractFilePath(parfilename);
    handle := windows.FindFirstFile(PChar(parfilename), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        dir := ExtractFilePath(Application.ExeName);
        parfilename := dir + '*.par';
        handle := windows.FindFirstFile(PChar(parfilename), fd);
    end;
    if (handle <> INVALID_HANDLE_VALUE) then begin
        windows.FindClose(handle);
        parfilename := dir + fd.cFileName;
        if (LoadCaliFile(PChar(parfilename))) then begin
            PanelBottomPar.Caption := fd.cFileName;
        end
    end
    else begin
        parfilename := '';
    end;

    tmpleft := ExtractFilePath(Application.ExeName) + '01l.jpg';
    tmpright := ExtractFilePath(Application.ExeName) + '01r.jpg';

    handle := windows.FindFirstFile(PChar(leftjpg), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        handle := windows.FindFirstFile(PChar(tmpleft), fd);
        leftjpg := tmpleft;
    end;

    if (handle <> INVALID_HANDLE_VALUE) then begin
        windows.FindClose(handle);
    end
    else begin
        leftjpg := '';
    end;

    handle := windows.FindFirstFile(PChar(rightjpg), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        handle := windows.FindFirstFile(PChar(tmpright), fd);
        rightjpg := tmpright;
    end;

    if (handle <> INVALID_HANDLE_VALUE) then begin
        windows.FindClose(handle);
    end
    else begin
        rightjpg := '';
    end;


    loadunrectifystereo(leftjpg, rightjpg);
    LoadAllFoto();
    fotospace_zoomall(0);
    fotospace_zoomall(1);

    mouseleftcount_l := 0;
    mouseleftcount_r := 0;

    TimerDelayRedraw.Enabled := True;
    PanelL.Cursor := MOUSECURSOR_HAND;
    PanelR.Cursor := MOUSECURSOR_HAND;
    PanelL.Tag := 1;
    PanelR.Tag := 1;
end;


procedure TFormMain.FormPaint(Sender: TObject);
begin
    TimerDelayRedraw.Enabled := True;
end;

procedure TFormMain.TimerDelayRedrawTimer(Sender: TObject);
begin
    fotospace_update(0);
    fotospace_update(1);
    TimerDelayRedraw.Enabled := False;
end;

procedure TFormMain.FormResize(Sender: TObject);
begin
    SBPreviousFoto.Left := 5;
    SBNextFoto.Left := PanelScroll.Width - SBNextFoto.Width - 5;
    TrackBarAllFotos.Left := SBPreviousFoto.Width + 10;
    TrackBarAllFotos.Width := PanelScroll.Width - SBPreviousFoto.Width - SBNextFoto.Width - 20;
    PanelWait.Left := Self.Width div 2 - PanelWait.Width div 2;
    PanelWait.Top := Self.Height div 2 - PanelWait.Height div 2;
    PanelBaseL.Width := Self.Width div 2 - 10;
    TimerZommAll.Enabled := True;
end;

procedure TFormMain.LoadMatchFotos();
begin
    if (matchhomodirty) then begin
        SetMatchFotos(PChar(realleftjpg), PChar(realrightjpg));
    end;

    matchhomodirty := False;
end;

procedure TFormMain.ResetTraceStatus();
begin
    fotospace_clearmeasureline(0);
    fotospace_clearmeasureline(1);
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    regionpnt_l := 0;
    regionpnt_r := 0;
    reselectregion_l := False;
    reselectregion_r := False;
end;

procedure TFormMain.PanelLMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
    scr_x, scr_y, fid: Integer;
    scr_x2, scr_y2: Integer;
    fotoleft1, fotoright1: tagPOINT;
    fotoleft2, fotoright2: tagPOINT;
    pnt_start, pnt_to: tagPOINT;
    txt: PChar;
    allowaddpnt: Boolean;
begin
    if (Button <> mbLeft) then begin
        ResetTraceStatus();
        fotospace_update(0);
        fotospace_update(1);
        Exit;
    end;

    if (PanelL.Tag = 0) then begin
        Self.LoadMatchFotos();
        mousepoints_x_l[mouseleftcount_l] := X;
        mousepoints_y_l[mouseleftcount_l] := Y;
        Inc(mouseleftcount_l);
        if (not Self.checkdistance()) then begin
            if (mouseleftcount_l = 3) then begin
                SaveMeasure2Lines();
            end
            else if ((mouseleftcount_l = 2) and (mouseleftcount_r = 0)) then begin
                //算出右边两个点
                fotospace_scr2pic(0, mousepoints_x_l[0], mousepoints_y_l[0], fotoleft1.x, fotoleft1.y);
                fotospace_scr2pic(0, X, Y, fotoleft2.x, fotoleft2.y);
                MatchHomoPoint(fotoleft1, fotoright1, False, CBAccurate.Checked);
                MatchHomoPoint(fotoleft2, fotoright2, False, CBAccurate.Checked);
                fotospace_pic2scr(1, fotoright1.x, fotoright1.y, scr_x, scr_y);
                fotospace_pic2scr(1, fotoright2.x, fotoright2.y, scr_x2, scr_y2);
                fotospace_drawmeasureline(1, scr_x, scr_y, scr_x2, scr_y2);
//                mouseleftcount_r := 2;
            end;
        end;
    end
    else if (PanelL.Tag = 1) then begin
        fotospace_panstart(0, X, Y);
        left_panstarted := True;
        if (CBBindZoom.Checked) then begin
            fotospace_panstart(1, X, Y);
        end;
    end
    else if (PanelL.Tag = 2) then begin
        fid := fotospace_selectbypoint(0, X, Y);
        if (fid >= 0) then begin
            fotospace_selectbyid(1, fid, pnt_start.x, pnt_start.y,
                pnt_to.x, pnt_to.y, txt);
            fotospace_update(0);
            fotospace_update(1);
        end;
    end
    else if (PanelL.Tag = 3) then begin
        //placemark
        Self.LoadMatchFotos();
        mousepoints_x_l[0] := X;
        mousepoints_y_l[0] := Y;
        mouseleftcount_l := 1;
        if (not placemark(0, X, Y)) then begin
            fotospace_scr2pic(0, X, Y, fotoleft1.x, fotoleft1.y);
            MatchHomoPoint(fotoleft1, fotoright1, False, CBAccurate.Checked);
            fotospace_pic2scr(1, fotoright1.x, fotoright1.y, scr_x, scr_y);
            fotospace_drawmeasureline(1, scr_x, scr_y, scr_x, scr_y);
        end;
    end
    else if (PanelL.Tag = 4) then begin
        //area
        if (regionpnt_l > 0) then begin
            Self.SaveMeasure2Lines();
        end
        else begin
            allowaddpnt := True;
            if (mouseleftcount_r > 0) and (regionpnt_r = 0) then begin
                //修改上一个点
                fotospace_removelastmeasuresegment(0);
                fotospace_addmeasuresegment(0, X, Y);
                fotospace_update(0);
                allowaddpnt := False;
            end
            else if (mouseleftcount_r = 0) and (regionpnt_r > 0) then begin
                if (not reselectregion_l) then begin
                    //重新选所有点
                    mouseleftcount_l := 0;
                    regionpnt_l := 0;
                    fotospace_clearmeasureline(0);
                    fotospace_update(0);
                    reselectregion_l := True;
                end;
                allowaddpnt := True;
            end;

            if (allowaddpnt) then begin
                Self.LoadMatchFotos();
                if (regionpnt_l = 0) then begin
                    mousepoints_x_l[mouseleftcount_l] := X;
                    mousepoints_y_l[mouseleftcount_l] := Y;
                    fotospace_addmeasuresegment(0, X, Y);
                    Inc(mouseleftcount_l);
                    fotospace_update(0);

                    //算出右边的同名点
                    if (regionpnt_r = 0) then begin
                        fotospace_scr2pic(0, X, Y, fotoleft1.x, fotoleft1.y);
                        MatchHomoPoint(fotoleft1, fotoright1, False, CBAccurate.Checked);
                        fotospace_pic2scr(1, fotoright1.x, fotoright1.y, scr_x, scr_y);
                        fotospace_addmeasuresegment(1, scr_x, scr_y);
                        fotospace_update(1);
                    end;
                end;
            end;
        end;
    end;
end;

procedure TFormMain.PanelRMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
    scr_x, scr_y, fid: Integer;
    scr_x2, scr_y2: Integer;
    fotoleft1, fotoright1: tagPOINT;
    fotoleft2, fotoright2: tagPOINT;
    pnt_start, pnt_to: tagPOINT;
    txt: PChar;
    allowaddpnt: Boolean;
begin
    if (Button <> mbLeft) then begin
        ResetTraceStatus();
        Exit;
    end;

    if (PanelR.Tag = 0) then begin
        Self.LoadMatchFotos();
        mousepoints_x_r[mouseleftcount_r] := X;
        mousepoints_y_r[mouseleftcount_r] := Y;
        Inc(mouseleftcount_r);
        if (not Self.checkdistance()) then begin
            if (mouseleftcount_r = 3) then begin
                SaveMeasure2Lines();
            end
            else if ((mouseleftcount_r = 2) and (mouseleftcount_l = 0)) then begin
                //算出left两个点
                fotospace_scr2pic(1, mousepoints_x_r[0], mousepoints_y_r[0], fotoright1.x, fotoright1.y);
                fotospace_scr2pic(1, X, Y, fotoright2.x, fotoright2.y);
                MatchHomoPoint(fotoright1, fotoleft1, True, CBAccurate.Checked);
                MatchHomoPoint(fotoright2, fotoleft2, True, CBAccurate.Checked);
                fotospace_pic2scr(0, fotoleft1.x, fotoleft1.y, scr_x, scr_y);
                fotospace_pic2scr(0, fotoleft2.x, fotoleft2.y, scr_x2, scr_y2);
                fotospace_drawmeasureline(0, scr_x, scr_y, scr_x2, scr_y2);
//                mouseleftcount_l := 2;
            end;
        end;
    end
    else if (PanelR.Tag = 1) then begin
        fotospace_panstart(1, X, Y);
        right_panstarted := True;
        if (CBBindZoom.Checked) then begin
            fotospace_panstart(0, X, Y);
        end;
    end
    else if (PanelR.Tag = 2) then begin
        fid := fotospace_selectbypoint(1, X, Y);
        if (fid >= 0) then begin
            fotospace_selectbyid(0, fid, pnt_start.x, pnt_start.y,
                pnt_to.x, pnt_to.y, txt);
            fotospace_update(0);
            fotospace_update(1);
        end;
    end
    else if (PanelL.Tag = 3) then begin
        //placemark
        Self.LoadMatchFotos();
        mousepoints_x_r[0] := X;
        mousepoints_y_r[0] := Y;
        mouseleftcount_r := 1;
        if (not placemark(1, X, Y)) then begin
            fotospace_scr2pic(1, X, Y, fotoright1.x, fotoright1.y);
            MatchHomoPoint(fotoright1, fotoleft1, True, CBAccurate.Checked);
            fotospace_pic2scr(0, fotoleft1.x, fotoleft1.y, scr_x, scr_y);
            fotospace_drawmeasureline(0, scr_x, scr_y, scr_x, scr_y);
        end;
    end
    else if (PanelR.Tag = 4) then begin
        //area
        if (regionpnt_r > 0) then begin
            Self.SaveMeasure2Lines();
        end
        else begin
            allowaddpnt := True;
            if (mouseleftcount_l > 0) and (regionpnt_l = 0) then begin
                //修改上一个点
                fotospace_removelastmeasuresegment(1);
                fotospace_addmeasuresegment(1, X, Y);
                fotospace_update(1);
                allowaddpnt := False;
            end
            else if (mouseleftcount_l = 0) and (regionpnt_l > 0) then begin
                if (not reselectregion_r) then begin
                    //重新选所有点
                    mouseleftcount_r := 0;
                    regionpnt_r := 0;
                    fotospace_clearmeasureline(1);
                    fotospace_update(1);
                    reselectregion_r := True;
                end;
                allowaddpnt := True;
            end;

            if (allowaddpnt) then begin
                Self.LoadMatchFotos();
                if (regionpnt_r = 0) then begin
                    mousepoints_x_r[mouseleftcount_r] := X;
                    mousepoints_y_r[mouseleftcount_r] := Y;
                    fotospace_addmeasuresegment(1, X, Y);
                    Inc(mouseleftcount_r);
                    fotospace_update(1);

                    //算出left边的同名点
                    if (regionpnt_l = 0) then begin
                        fotospace_scr2pic(1, X, Y, fotoleft1.x, fotoleft1.y);
                        MatchHomoPoint(fotoleft1, fotoright1, False, CBAccurate.Checked);
                        fotospace_pic2scr(0, fotoright1.x, fotoright1.y, scr_x, scr_y);
                        fotospace_addmeasuresegment(0, scr_x, scr_y);
                        fotospace_update(0);
                    end;
                end;
            end;
        end;
    end;
end;

procedure TFormMain.PanelLMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
    if (PanelL.Tag = 0) then begin
        if (mouseleftcount_l = 1) then begin
            fotospace_drawmeasureline(0, mousepoints_x_l[0], mousepoints_y_l[0], X, Y);
        end;
    end
    else if (PanelL.Tag = 4) and (regionpnt_l = 0) then begin
        if (mouseleftcount_l > 0) then begin
            fotospace_addmeasuresegment(0, X, Y);
            fotospace_update(0);
            fotospace_removelastmeasuresegment(0);
        end;
    end
    else if ((PanelL.Tag = 1) and (left_panstarted)) then begin
        fotospace_panmoveTo(0, X, Y);
        if (CBBindZoom.Checked) then begin
            fotospace_panmoveTo(1, X, Y);
        end;
    end;
end;

procedure TFormMain.PanelRMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
    if (PanelR.Tag = 0) then begin
        if (mouseleftcount_r = 1) then begin
            fotospace_drawmeasureline(1, mousepoints_x_r[0], mousepoints_y_r[0], X, Y);
        end;
    end
    else if (PanelR.Tag = 4) and (regionpnt_r = 0) then begin
        if (mouseleftcount_r > 0) then begin
            fotospace_addmeasuresegment(1, X, Y);
            fotospace_update(1);
            fotospace_removelastmeasuresegment(1);
        end;
    end
    else if ((PanelR.Tag = 1) and (right_panstarted)) then begin
        fotospace_panmoveTo(1, X, Y);
        if (CBBindZoom.Checked) then begin
            fotospace_panmoveTo(0, X, Y);
        end;
    end;
end;

procedure TFormMain.PanelLMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
    if ((PanelL.Tag = 1) and (Button = mbLeft)) then begin
        fotospace_panstop(0);
        left_panstarted := False;
        Self.bindrightpanel(0);
    end;
end;

procedure TFormMain.PanelRMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
    if (PanelR.Tag = 1) then begin
        fotospace_panstop(1);
        right_panstarted := False;
        Self.bindrightpanel(1);
    end
end;

procedure TFormMain.FormCreate(Sender: TObject);
begin
    //load cursor resource 鼠标光标资源
    Screen.Cursors[MOUSECURSOR_DEFAULT]     := LoadCursor(HInstance, 'IDC_DEFAULT');
    Screen.Cursors[MOUSECURSOR_ARROW1]      := LoadCursor(HInstance, 'IDC_ARROW1');
    Screen.Cursors[MOUSECURSOR_ARROW2]      := LoadCursor(HInstance, 'IDC_ARROW2');
    Screen.Cursors[MOUSECURSOR_CROSS1]      := LoadCursor(HInstance, 'IDC_CROSS1');
    Screen.Cursors[MOUSECURSOR_CROSS2]      := LoadCursor(HInstance, 'IDC_CROSS2');
    Screen.Cursors[MOUSECURSOR_CROSSBOX]    := LoadCursor(HInstance, 'IDC_CROSSBOX');
    Screen.Cursors[MOUSECURSOR_PICKUP]      := LoadCursor(HInstance, 'IDC_PICKUP');
    Screen.Cursors[MOUSECURSOR_MOVE]        := LoadCursor(HInstance, 'IDC_MOVE');
    Screen.Cursors[MOUSECURSOR_COPY]        := LoadCursor(HInstance, 'IDC_COPY');
    Screen.Cursors[MOUSECURSOR_DRAG]        := LoadCursor(HInstance, 'IDC_DRAG');
    Screen.Cursors[MOUSECURSOR_BEAM]        := LoadCursor(HInstance, 'IDC_BEAM');
    Screen.Cursors[MOUSECURSOR_CANCEL]      := LoadCursor(HInstance, 'IDC_CANCEL');
    Screen.Cursors[MOUSECURSOR_MEASURE]     := LoadCursor(HInstance, 'IDC_MEASURE');
    Screen.Cursors[MOUSECURSOR_HAND]        := LoadCursor(HInstance, 'IDC_HAND');
    Screen.Cursors[MOUSECURSOR_ZOOM]        := LoadCursor(HInstance, 'IDC_ZOOM');
    Screen.Cursors[MOUSECURSOR_ZOOMIN]      := LoadCursor(HInstance, 'IDC_ZOOMIN');
    Screen.Cursors[MOUSECURSOR_ZOOMOUT]     := LoadCursor(HInstance, 'IDC_ZOOMOUT');
    Screen.Cursors[MOUSECURSOR_BUSYWAIT]    := LoadCursor(HInstance, 'IDC_BUSYWAIT');
    Screen.Cursors[MOUSECURSOR_BARRIER]     := LoadCursor(HInstance, 'IDC_BARRIER');
    Screen.Cursors[MOUSECURSOR_FOX]         := LoadCursor(HInstance, 'IDC_FOX');
    Screen.Cursors[MOUSECURSOR_PIGGY]       := LoadCursor(HInstance, 'IDC_PIGGY');
    Screen.Cursors[MOUSECURSOR_HOLD]        := LoadCursor(HInstance, 'IDC_HOLD');
    Screen.Cursors[MOUSECURSOR_CATCH]       := LoadCursor(HInstance, 'IDC_CATCH');
    Screen.Cursors[MOUSECURSOR_PENCIL]      := LoadCursor(HInstance, 'IDC_PENCIL');
    Screen.Cursors[MOUSECURSOR_BULLSEYE]    := LoadCursor(HInstance, 'IDC_BULLSEYE');
    Screen.Cursors[MOUSECURSOR_TERMINATOR]  := LoadCursor(HInstance, 'IDC_TERMINATOR');
    Screen.Cursors[MOUSECURSOR_PALETTE]     := LoadCursor(HInstance, 'IDC_PALETTE');

    frmplacemark := TFormPlacemark.Create(Self);
    frmCalibration1 := TFormCalibration1.Create(Self);
    frmCalibration2 := TFormCalibration2.Create(Self);
    matchhomodirty := False;

    fotos_l := TStringList.Create();
    fotos_r := TStringList.Create();
end;

procedure TFormMain.FormMouseWheelDown(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
var
    scale: Double;
begin
    if (SBMZoom.Visible) then begin
        fotospace_getscale(0, scale);
        if (not fotospace_setscale(0, scale*1.1)) then begin
            Exit;
        end;
        fotospace_update(0);

        fotospace_getscale(1, scale);
        fotospace_setscale(1, scale*1.1);
        fotospace_update(1);
    end
    else begin
        if (not TimerNextFoto.Enabled) then begin
            TimerNextFoto.Enabled := True;
        end;
    end;
end;

procedure TFormMain.FormMouseWheelUp(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
var
    scale: Double;
begin
    if (SBMZoom.Visible) then begin
        fotospace_getscale(0, scale);
        if (not fotospace_setscale(0, scale/1.1)) then begin
            Exit;
        end;
        fotospace_update(0);

        fotospace_getscale(1, scale);
        fotospace_setscale(1, scale/1.1);
        fotospace_update(1);
    end
    else begin
        if (not TimerPreviousFoto.Enabled) then begin
            TimerPreviousFoto.Enabled := True;
        end;
    end;
end;

procedure TFormMain.TimerZommAllTimer(Sender: TObject);
begin
    Self.SpeedButton6Click(Sender);
    TimerZommAll.Enabled := False;
end;

procedure TFormMain.SpeedButton1Click(Sender: TObject);
begin
    OpenDialogPar.InitialDir := ExtractFilePath(Application.ExeName);
    if (not OpenDialogPar.Execute()) then begin
        Exit;
    end;

    PanelBottomPar.Caption := '<no par>';
    parfilename := AnsiString(OpenDialogPar.FileName);
    if (not LoadCaliFile(PChar(parfilename))) then begin
        MessageBox(0, 'failed .par', '', MB_OK);
        Exit;
    end;

    PanelBottomPar.Caption := ExtractFileName(OpenDialogPar.FileName);
end;

procedure TFormMain.LoadLeftFoto(leftfoto: AnsiString);
var
    ljpg, rjpg: AnsiString;
    dir: AnsiString;
    fd: TWin32FindData;
    handle: THandle;
begin
    handle := windows.FindFirstFile(PChar(leftfoto), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        ShowMessage('can''t find the foto.');
        Exit;
    end;
    windows.FindClose(handle);


    ljpg := leftfoto;

    rjpg := GetRightFoto(ljpg);
    if (rjpg = '') then begin
        OpenDialogJpg.Title := 'open the right foto';
        if (not OpenDialogJpg.Execute()) then begin
            Exit;
        end;
        rjpg := OpenDialogJpg.FileName;
    end;

    fotos_l.Clear();
    fotos_r.Clear();

    //reload par
    dir := ExtractFilePath(ljpg);
    //先查找同级目录
    handle := windows.FindFirstFile(PChar(dir + '*.par'), fd);
    if (handle = INVALID_HANDLE_VALUE) then begin
        //上一级目录
        Delete(dir, Length(dir), 1);
        dir := ExtractFilePath(dir);
        handle := windows.FindFirstFile(PChar(dir + '*.par'), fd);
        if (handle = INVALID_HANDLE_VALUE) then begin
            //上上一级目录
            Delete(dir, Length(dir), 1);
            dir := ExtractFilePath(dir);
            handle := windows.FindFirstFile(PChar(dir + '*.par'), fd);
        end;
    end;

    if (handle <> INVALID_HANDLE_VALUE) then begin
        windows.FindClose(handle);
        PanelBottomPar.Caption := '<no par>';
        parfilename := AnsiString(dir + fd.cFileName);
        if (LoadCaliFile(PChar(parfilename))) then begin
            PanelBottomPar.Caption := fd.cFileName;
        end;
    end;

    loadunrectifystereo(ljpg, rjpg);
    LoadAllFoto();
    AddLoadLeftName(ljpg);
    
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;

    fotospace_zoomall(0);
    fotospace_zoomall(1);
    TimerDelayRedraw.Enabled := True;
end;

procedure TFormMain.SBLoadStereoClick(Sender: TObject);
begin
    OpenDialogJpg.Title := 'open the left foto';
    if (not OpenDialogJpg.Execute()) then begin
        Exit;
    end;

    LoadLeftFoto(OpenDialogJpg.FileName);
end;

procedure TFormMain.SpeedButton11Click(Sender: TObject);
begin
    fotospace_clearmeasureline(0);
    fotospace_clearmeasureline(1);
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    fotospace_clear(0, True);
    fotospace_clear(1, True);
    fotospace_savemeasures(0, PChar(leftfsm));
    fotospace_savemeasures(1, PChar(rightfsm));
    fotospace_update(0);
    fotospace_update(1);
end;

procedure TFormMain.SpeedButton5Click(Sender: TObject);
begin
//    mouseleftcount_l := 0;
//    mouseleftcount_r := 0;
    PanelL.Tag := 1;
    PanelR.Tag := 1;
    PanelL.Cursor := MOUSECURSOR_HAND;
    PanelR.Cursor := MOUSECURSOR_HAND;
end;

procedure TFormMain.SpeedButton7Click(Sender: TObject);
begin
    if (PanelL.Tag <> 1) then begin
        mouseleftcount_l := 0;
    end;
    if (PanelR.Tag <> 1) then begin
        mouseleftcount_r := 0;
    end;
    PanelL.Tag := 0;
    PanelR.Tag := 0;
    PanelL.Cursor := MOUSECURSOR_ARROW2;
    PanelR.Cursor := MOUSECURSOR_ARROW2;
end;

procedure TFormMain.SpeedButton10Click(Sender: TObject);
begin
    fotospace_deleteselection(0);
    fotospace_deleteselection(1);
    fotospace_savemeasures(0, PChar(leftfsm));
    fotospace_savemeasures(1, PChar(rightfsm));
    fotospace_update(0);
    fotospace_update(1);
end;

procedure TFormMain.SpeedButton3Click(Sender: TObject);
var
    scale: Double;
begin
    fotospace_getscale(0, scale);
    fotospace_setscale(0, scale*2);
    fotospace_update(0);

    fotospace_getscale(1, scale);
    fotospace_setscale(1, scale*2);
    fotospace_update(1);
end;

procedure TFormMain.SpeedButton4Click(Sender: TObject);
var
    scale: Double;
begin
    fotospace_getscale(0, scale);
    fotospace_setscale(0, scale/2);
    fotospace_update(0);
    fotospace_getscale(1, scale);
    fotospace_setscale(1, scale/2);
    fotospace_update(1);
end;

procedure TFormMain.SpeedButton6Click(Sender: TObject);
begin
    fotospace_zoomall(0);
    fotospace_update(0);
    bindrightpanel(0);
    fotospace_zoomall(1);
    fotospace_update(1);
end;

procedure TFormMain.SpeedButton9Click(Sender: TObject);
begin
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    PanelL.Tag := 2;
    PanelR.Tag := 2;
    PanelL.Cursor := MOUSECURSOR_PICKUP;
    PanelR.Cursor := MOUSECURSOR_PICKUP;
end;

procedure TFormMain.SpeedButton8Click(Sender: TObject);
begin
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    PanelL.Tag := 3;
    PanelR.Tag := 3;
    PanelL.Cursor := MOUSECURSOR_CROSS2;
    PanelR.Cursor := MOUSECURSOR_CROSS2;
    if (not frmplacemark.Visible) then begin
        frmplacemark.Left := Self.Left +30;
        frmplacemark.Top := Self.Top + 65;
        frmplacemark.Show();
        Self.SetFocus();
    end;
end;

procedure TFormMain.SBPreviousFotoClick(Sender: TObject);
var
    l, r: AnsiString;
begin
    TimerRectify.Enabled := False;
    if (GetNextFotoNames(l, r, True)) then begin
        ResetTraceStatus();
        self.leftjpg := l;
        self.rightjpg := r;
        loadthumbnails(l, r);
        fotospace_zoomall(0);
        fotospace_zoomall(1);
        fotospace_update(0);
        fotospace_update(1);
        TrackBarAllFotos.Tag := 1;
        TrackBarAllFotos.Position := TrackBarAllFotos.Position-1;
        TrackBarAllFotos.Tag := 0;
    end;
    TimerRectify.Enabled := True;
end;

procedure TFormMain.TimerRectifyTimer(Sender: TObject);
begin
    loadunrectifystereo(leftjpg, rightjpg);
    fotospace_zoomall(0);
    fotospace_zoomall(1);
    fotospace_update(0);
    fotospace_update(1);
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    TimerRectify.Enabled := False;
end;

procedure TFormMain.SBNextFotoClick(Sender: TObject);
var
    l, r: AnsiString;
begin
    TimerRectify.Enabled := False;
    if (GetNextFotoNames(l, r, False)) then begin
        ResetTraceStatus();
        self.leftjpg := l;
        self.rightjpg := r;
        loadthumbnails(l, r);
        fotospace_zoomall(0);
        fotospace_zoomall(1);
        fotospace_update(0);
        fotospace_update(1);
        TrackBarAllFotos.Tag := 1;
        TrackBarAllFotos.Position := TrackBarAllFotos.Position+1;
        TrackBarAllFotos.Tag := 0;
    end;
    TimerRectify.Enabled := True;
end;

procedure TFormMain.SBMZoomClick(Sender: TObject);
begin
    SBMZoom.Visible := False;
    SBMBrowse.Visible := True;
end;

procedure TFormMain.SBMBrowseClick(Sender: TObject);
begin
    SBMZoom.Visible := True;
    SBMBrowse.Visible := False;
end;

procedure TFormMain.FormClose(Sender: TObject; var Action: TCloseAction);
var
    strlist: TStringList;
    i, count: Longint;
begin
    Action := caFree;
    strlist := TStringList.Create();
    strlist.Add(parfilename);
    strlist.Add(leftjpg);
    strlist.Add(rightjpg);
    strlist.Add(FrmPlacemark.Edit1.Text);
    //写入recent list
    count := PopupMenuRecentList.Items.Count;
    if (count > 10) then begin
        count := 10;
    end;
    strlist.Add(IntToStr(count));
    for i:=0 to count-1 do begin
        strlist.Add(PopupMenuRecentList.Items[i].Caption);
    end;

    strlist.SaveToFile(ExtractFilePath(Application.ExeName)
        + ChangeFileExt(ExtractFileName(Application.ExeName), '.cfg'));

    strlist.Free();
    frmplacemark.Free();
    frmCalibration1.Free();
    frmCalibration2.Free();
    fotos_l.Free();
    fotos_r.Free();
end;

procedure TFormMain.TimerPreviousFotoTimer(Sender: TObject);
begin
    SBPreviousFotoClick(Sender);
    TimerPreviousFoto.Enabled := False;
end;

procedure TFormMain.TimerNextFotoTimer(Sender: TObject);
begin
    SBNextFotoClick(Sender);
    TimerNextFoto.Enabled := False;
end;

procedure TFormMain.CBRectifyClick(Sender: TObject);
begin
    TimerRectify.Enabled := True;
    PanelBottom.SetFocus();
end;

procedure TFormMain.SBMatchClick(Sender: TObject);
begin
    SaveMeasure2Lines();
end;

procedure TFormMain.SBUnCancelTrackClick(Sender: TObject);
begin
    if ((mouseleftcount_l = 0) and (mouseleftcount_r = 0)) then begin
        fotospace_deletelatest(0, False);
        fotospace_deletelatest(1, False);
    end;

    fotospace_clearmeasureline(0);
    fotospace_clearmeasureline(1);
    mouseleftcount_l := 0;
    mouseleftcount_r := 0;
    fotospace_update(0);
    fotospace_update(1);
    fotospace_savemeasures(0, PChar(leftfsm));
    fotospace_savemeasures(1, PChar(rightfsm));
end;

procedure TFormMain.SBCalibrateClick(Sender: TObject);
var
    pnt: TPoint;
begin
    pnt.x := SBCalibrate.Left;
    pnt.y := SBCalibrate.Top+SBCalibrate.Height;
    pnt := Self.ClientToScreen(pnt);
    PopupMenuCalibrate.Popup(pnt.x, pnt.y);
end;

procedure TFormMain.CalibrateSeparate1Click(Sender: TObject);
var
    leftfotos, reportfile: AnsiString;
    rightfotos: AnsiString;
    r: AnsiString;
    lines: TStringList;
begin
    frmCalibration2.ShowModal();
    if (not frmCalibration2.Tag = 1) then begin
        Exit;
    end;
    leftfotos := frmCalibration2.GetLeftFotos();
    rightfotos := frmCalibration2.GetRightFotos();
    r := StereoCalib(PChar(leftfotos), PChar(rightfotos), frmCalibration2.chessboard_width.Value,
        frmCalibration2.chessboard_height.Value, StrToFloat(frmCalibration2.squaresize.Text),
        StrToFloat(frmCalibration2.ccdpixelsize.Text)/1000.0, nil, StereoCalibcallback);

    if (r = '') then begin
        Exit;
    end;
    lines := TStringList.Create();
    lines.SetText(PChar(r));
//    ShowMessage('completed: "' + lines.Strings[0] +'"');
    reportfile := ChangeFileExt(lines.Strings[0], '') + '.report.txt';
    lines.SaveToFile(reportfile);
    lines.Free();

    reportfile := 'notepad ' + reportfile;
    WinExec(PChar(reportfile), SW_SHOWNORMAL);
    FormMain.PanelWait.Visible := False;
end;

procedure TFormMain.CalibrateMixedFotos1Click(Sender: TObject);
var
    fotos, reportfile: AnsiString;
    r: AnsiString;
    lines: TStringList;
begin
    frmCalibration1.ShowModal();
    if (not frmCalibration1.Tag = 1) then begin
        Exit;
    end;
    fotos := frmCalibration1.GetFotos();
    r := StereoCalib1(PChar(fotos), frmCalibration1.chessboard_width.Value,
        frmCalibration1.chessboard_height.Value, StrToFloat(frmCalibration1.squaresize.Text),
        StrToFloat(frmCalibration1.ccdpixelsize.Text)/1000.0, nil, StereoCalibcallback);

    if (r = '') then begin
        Exit;
    end;
    lines := TStringList.Create();
    lines.SetText(PChar(r));
//    ShowMessage('completed: "' + lines.Strings[0] +'"');
    reportfile := ChangeFileExt(lines.Strings[0], '') + '.report.txt';
    lines.SaveToFile(reportfile);
    lines.Free();

    reportfile := 'notepad ' + reportfile;
    WinExec(PChar(reportfile), SW_SHOWNORMAL);
    FormMain.PanelWait.Visible := False;
end;

procedure TFormMain.SpeedButton12Click(Sender: TObject);
begin
    if (PanelL.Tag <> 1) then begin
        mouseleftcount_l := 0;
        regionpnt_l := 0;
        reselectregion_l := False;
    end;
    if (PanelR.Tag <> 1) then begin
        mouseleftcount_r := 0;
        regionpnt_r := 0;
        reselectregion_r := False;
    end;
    PanelL.Tag := 4;
    PanelR.Tag := 4;
    PanelL.Cursor := MOUSECURSOR_PENCIL;
    PanelR.Cursor := MOUSECURSOR_PENCIL;
end;

procedure TFormMain.PanelLDblClick(Sender: TObject);
begin
    if (PanelL.Tag = 4) and (mouseleftcount_l > 3) then begin
        TimerRemoveLast2Left.Enabled := True;
    end;
end;

procedure TFormMain.TimerRemoveLast2LeftTimer(Sender: TObject);
begin
    fotospace_removelastmeasuresegment(0);
    fotospace_removelastmeasuresegment(0);
    fotospace_update(0);
    if (regionpnt_r = 0) then begin
        fotospace_removelastmeasuresegment(1);
        fotospace_removelastmeasuresegment(1);
        fotospace_update(1);
    end;
    Dec(mouseleftcount_l, 2);
    regionpnt_l := mouseleftcount_l;//记录点个数
    mouseleftcount_l := 0;
    reselectregion_l := False;
    TimerRemoveLast2Left.Enabled := False;
end;

procedure TFormMain.PanelRDblClick(Sender: TObject);
begin
    if (PanelR.Tag = 4) and (mouseleftcount_r > 3) then begin
        TimerRemoveLast2Right.Enabled := True;
    end;
end;

procedure TFormMain.TimerRemoveLast2RightTimer(Sender: TObject);
begin
    fotospace_removelastmeasuresegment(1);
    fotospace_removelastmeasuresegment(1);
    fotospace_update(1);
    if (regionpnt_l = 0) then begin
        fotospace_removelastmeasuresegment(0);
        fotospace_removelastmeasuresegment(0);
        fotospace_update(0);
    end;
    Dec(mouseleftcount_r, 2);
    regionpnt_r := mouseleftcount_r;//记录点个数
    mouseleftcount_r := 0;
    reselectregion_r := False;
    TimerRemoveLast2Right.Enabled := False;
end;

procedure TFormMain.TrackBarAllFotosEnter(Sender: TObject);
begin
    PanelBottom.SetFocus();
end;

procedure TFormMain.LoadAllFoto();
var
    l, r: AnsiString;
    currentleft, currentright: AnsiString;
    index: Longint;
begin
    TrackBarAllFotos.Tag := 1;
    currentleft := leftjpg;
    currentright := rightjpg;
    fotos_l.Clear();
    fotos_l.Clear();
    TrackBarAllFotos.Position := 0;
    TrackBarAllFotos.Max := 0;
    index := 0;
    //先向前找,找到头
    while (GetNextFotoNames(l, r, True)) do begin
        leftjpg := l;
        rightjpg := r;
        index := index-1;
    end;

    //再从头到尾找
    fotos_l.Add(leftjpg);
    fotos_r.Add(rightjpg);
    while (GetNextFotoNames(l, r, False)) do begin
        leftjpg := l;
        rightjpg := r;
        fotos_l.Add(l);
        fotos_r.Add(r);
        TrackBarAllFotos.Max := TrackBarAllFotos.Max+1;
    end;
    TrackBarAllFotos.Position := -index;

    leftjpg := currentleft;
    rightjpg := currentright;
    TrackBarAllFotos.Tag := 0;
    PanelBottomRightSide.Caption := IntToStr(TrackBarAllFotos.Position) + '/' + IntToStr(TrackBarAllFotos.Max);
end;

procedure TFormMain.TrackBarAllFotosChange(Sender: TObject);
begin
    PanelBottomRightSide.Caption := IntToStr(TrackBarAllFotos.Position) + '/' + IntToStr(TrackBarAllFotos.Max);
    if (TrackBarAllFotos.Tag <> 0) then begin
        Exit;
    end;

    TimerRectify.Enabled := False;
    ResetTraceStatus();
    self.leftjpg := fotos_l[TrackBarAllFotos.Position];
    self.rightjpg := fotos_r[TrackBarAllFotos.Position];
    loadthumbnails(self.leftjpg, self.rightjpg);
    fotospace_zoomall(0);
    fotospace_zoomall(1);
    fotospace_update(0);
    fotospace_update(1);
    TimerRectify.Enabled := True;
end;

procedure TFormMain.AddLoadLeftName(leftname: AnsiString);
var
    i: Longint;
    namelist: TStringList;
    newname, itemname: AnsiString;
    menuitem: TMenuItem;
begin
    namelist := TStringList.Create();
    newname := LowerCase(leftname);
    namelist.Add(newname);
    for i:=0 to PopupMenuRecentList.Items.Count-1 do begin
        itemname := LowerCase(PopupMenuRecentList.Items[i].Caption);
        if (newname <> itemname) then begin
            namelist.Add(itemname);
        end;
    end;

    PopupMenuRecentList.Items.Clear();
    for i:=0 to namelist.Count-1 do begin
        menuitem := TMenuItem.Create(PopupMenuRecentList);
        menuitem.Caption := namelist[i];
        menuitem.OnClick := RecentItemClick;
        PopupMenuRecentList.Items.Add(menuitem);
    end;

    if (namelist.Count > 0) then begin
        SBLoadStereo.Hint :=  namelist.GetText;
    end;

    namelist.Free();
end;

procedure TFormMain.RecentItemClick(Sender: TObject);
var
    menuitem: TMenuItem;
begin
    menuitem := (Sender as TMenuItem);
    LoadLeftFoto(menuitem.Caption);
end;

procedure TFormMain.CBBindZoomClick(Sender: TObject);
begin
    PanelBottom.SetFocus();
end;

procedure TFormMain.CBAccurateClick(Sender: TObject);
begin
    PanelBottom.SetFocus();
end;

end.

