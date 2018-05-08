unit UnitCalibration2;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Spin;

type
  TFormCalibration2 = class(TForm)
    chessboard_width: TSpinEdit;
    Label2: TLabel;
    chessboard_height: TSpinEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    BtnOK: TButton;
    Button2: TButton;
    BtnLoadLeft: TButton;
    BtnClear: TButton;
    OpenDialog1: TOpenDialog;
    LBLeft: TListBox;
    LBRight: TListBox;
    BtnLoadRight: TButton;
    squaresize: TEdit;
    Label6: TLabel;
    Label7: TLabel;
    ccdpixelsize: TEdit;
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure BtnOKClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure BtnLoadLeftClick(Sender: TObject);
    procedure BtnLoadRightClick(Sender: TObject);
    procedure BtnClearClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

    fotos_l, fotos_r: TStrings;
    function GetLeftFotos(): AnsiString;
    function GetRightFotos(): AnsiString;

  end;

var
  FormCalibration2: TFormCalibration2;

implementation

{$R *.DFM}

function TFormCalibration2.GetLeftFotos(): AnsiString;
begin
    Result := fotos_l.GetText();
end;

function TFormCalibration2.GetRightFotos(): AnsiString;
begin
    Result := fotos_r.GetText();
end;

procedure TFormCalibration2.FormShow(Sender: TObject);
begin
    Self.Tag := 0;
end;

procedure TFormCalibration2.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
    Action := caHide;
end;

procedure TFormCalibration2.BtnOKClick(Sender: TObject);
begin
    Self.Tag := 1;
    Self.Close();
end;

procedure TFormCalibration2.Button2Click(Sender: TObject);
begin
    Self.Tag := 0;
    Self.Close();
end;

procedure TFormCalibration2.BtnLoadLeftClick(Sender: TObject);
var
    i: Longint;
    filename: AnsiString;
begin
    if (not OpenDialog1.Execute()) then begin
        Exit;
    end;

    LBLeft.Items.Clear();
    fotos_l.Clear();
    for i := 0 to OpenDialog1.Files.Count-1 do begin
        filename := OpenDialog1.Files.Strings[i];
        fotos_l.Add(filename);
        filename := ExtractFileName(filename);
        LBLeft.Items.Add(filename);
    end;
end;

procedure TFormCalibration2.BtnLoadRightClick(Sender: TObject);
var
    i: Longint;
    filename: AnsiString;
begin
    if (not OpenDialog1.Execute()) then begin
        Exit;
    end;

    LBRight.Items.Clear();
    fotos_r.Clear();
    for i := 0 to OpenDialog1.Files.Count-1 do begin
        filename := OpenDialog1.Files.Strings[i];
        fotos_r.Add(filename);
        filename := ExtractFileName(filename);
        LBRight.Items.Add(filename);
    end;
end;

procedure TFormCalibration2.BtnClearClick(Sender: TObject);
begin
    fotos_l.Clear();
    LBLeft.Items.Clear();
    fotos_r.Clear();
    LBRight.Items.Clear();
end;

procedure TFormCalibration2.FormCreate(Sender: TObject);
begin
    fotos_l := TStringList.Create();
    fotos_r := TStringList.Create();
end;

procedure TFormCalibration2.FormDestroy(Sender: TObject);
begin
    fotos_l.Free();
    fotos_r.Free();
end;

end.
