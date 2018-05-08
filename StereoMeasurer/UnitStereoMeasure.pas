unit UnitStereoMeasure;

interface

uses
    Windows;

type
    TStereoCalibcallback = function(progress: Longint; msg: PChar; fotofilename: PChar;
        isleft: Boolean): Boolean; stdcall;

type
    TStereoCalib = function(leftfotolist: PChar; rightfotolist: PChar;
        chessboard_width: Longint; chessboard_height: Longint;
        squaresize: Double; ccdpixelsize: Double; calibfile: PChar;
        callback: TStereoCalibcallback): PChar; stdcall;

    TStereoCalib1 = function(fotolist: PChar; chessboard_width: Longint;
        chessboard_height: Longint; squaresize: Double; ccdpixelsize: Double;
        calibfile: PChar; callback: TStereoCalibcallback): PChar; stdcall;

    TLoadCaliFile = function(filename: PChar): Boolean; stdcall;

    TRecalifyImages = function(ori_left: PChar; ori_right: PChar;
        cal_left: PChar; cal_right: PChar): Boolean; stdcall;

    TUndistortedImage = function(ori: PChar; undistorted: PChar;
        isleft: Boolean): Boolean; stdcall;

    TMeasurePosition = function(left_x: Integer; left_y: Integer; right_x: Integer;
        right_y: Integer): Double; stdcall;

    TMeasureDistance = function(left_x1: Longint; left_y1: Longint;
        left_x2: Longint; left_y2: Longint; right_x1: Longint; right_y1: Longint;
        right_x2: Longint; right_y2: Longint): Double; stdcall;

    TMeasureDistance1 = function(left_pt1: PInteger; right_pt1: PInteger;
        left_pt2: PInteger; right_pt2: PInteger): Double; stdcall;

    TMeasureAreaTriangle = function(left_x1: Longint; left_y1: Longint;
        left_x2: Longint; left_y2: Longint; left_x3: Longint; left_y3: Longint;
        right_x1: Longint; right_y1: Longint; right_x2: Longint; right_y2: Longint;
        right_x3: Longint; right_y3: Longint): Double; stdcall;

    TMeasureAreaTriangle1 = function(left_pt1: PInteger; right_pt1: PInteger;
        left_pt2: PInteger; right_pt2: PInteger; left_pt3: PInteger; right_pt3: PInteger): Double; stdcall;

    TMeasureAngle = function(left_pt1: PInteger; right_pt1: PInteger;
        left_pt2: PInteger; right_pt2: PInteger; left_pt3: PInteger;
        right_pt3: PInteger): Double; stdcall;

    TMatchHomoPointWithFotos = function(srcfoto: PChar; targetfoto: PChar;
        homopntsrc: tagPOINT; out homopnttarget: tagPOINT; accurate: Boolean): Boolean; stdcall;

    TSetMatchFotos = function(srcfoto: PChar; targetfoto: PChar): Boolean; stdcall;

    TMatchHomoPoint = function(homopntsrc: tagPOINT; out homopnttarget: tagPOINT;
        swapfotos: Boolean; accurate: Boolean): Boolean; stdcall;

var
    stereomeasuremodule: HMODULE;
    StereoCalib: TStereoCalib;
    StereoCalib1: TStereoCalib1;
    LoadCaliFile: TLoadCaliFile;
    RecalifyImages: TRecalifyImages;
    UndistortedImage: TUndistortedImage;
    MeasurePosition: TMeasurePosition;
    MeasureDistance: TMeasureDistance;
    MeasureDistance1: TMeasureDistance1;
    MeasureAreaTriangle: TMeasureAreaTriangle;
    MeasureAreaTriangle1: TMeasureAreaTriangle1;
    MeasureAngle: TMeasureAngle;
    MatchHomoPointWithFotos: TMatchHomoPointWithFotos;
    SetMatchFotos: TSetMatchFotos;
    MatchHomoPoint: TMatchHomoPoint;

implementation
                         

function InitialStereoMeasureLib(const filename: PChar): Boolean;
begin
    Result := False;
    stereomeasuremodule := LoadLibrary(filename);
    if (0 = stereomeasuremodule) then begin
        Exit;
    end;

    StereoCalib := TStereoCalib(GetProcAddress(stereomeasuremodule, 'StereoCalib'));
    StereoCalib1 := TStereoCalib1(GetProcAddress(stereomeasuremodule, 'StereoCalib1'));
    LoadCaliFile := TLoadCaliFile(GetProcAddress(stereomeasuremodule, 'LoadCaliFile'));
    RecalifyImages := TRecalifyImages(GetProcAddress(stereomeasuremodule, 'RecalifyImages'));
    UndistortedImage := TUndistortedImage(GetProcAddress(stereomeasuremodule, 'UndistortedImage'));
    MeasurePosition := TMeasurePosition(GetProcAddress(stereomeasuremodule, 'MeasurePosition'));
    MeasureDistance := TMeasureDistance(GetProcAddress(stereomeasuremodule, 'MeasureDistance'));
    MeasureDistance1 := TMeasureDistance1(GetProcAddress(stereomeasuremodule, 'MeasureDistance1'));
    MeasureAreaTriangle := TMeasureAreaTriangle(GetProcAddress(stereomeasuremodule, 'MeasureAreaTriangle'));
    MeasureAreaTriangle1 := TMeasureAreaTriangle1(GetProcAddress(stereomeasuremodule, 'MeasureAreaTriangle1'));
    MeasureAngle := TMeasureAngle(GetProcAddress(stereomeasuremodule, 'MeasureAngle'));
    MatchHomoPointWithFotos := TMatchHomoPointWithFotos(GetProcAddress(stereomeasuremodule, 'MatchHomoPointWithFotos'));
    SetMatchFotos := TSetMatchFotos(GetProcAddress(stereomeasuremodule, 'SetMatchFotos'));
    MatchHomoPoint := TMatchHomoPoint(GetProcAddress(stereomeasuremodule, 'MatchHomoPoint'));

    Result := True;
end;

////////////////////////////////////////////////////////////////////////////////
initialization
    InitialStereoMeasureLib('StereoMeasure.dll');

finalization
    if (0 <> stereomeasuremodule) then begin
        FreeLibrary(stereomeasuremodule);
    end;


end.
