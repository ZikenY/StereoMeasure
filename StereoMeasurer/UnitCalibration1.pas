unit UnitCalibration1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Spin;

type
  TFormCalibration1 = class(TForm)
    ListBox1: TListBox;
    Label1: TLabel;
    chessboard_width: TSpinEdit;
    Label2: TLabel;
    chessboard_height: TSpinEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    BtnOK: TButton;
    Button2: TButton;
    BtnLoad: TButton;
    BtnClear: TButton;
    OpenDialog1: TOpenDialog;
    squaresize: TEdit;
    Label6: TLabel;
    Label7: TLabel;
    ccdpixelsize: TEdit;
    procedure BtnLoadClick(Sender: TObject);
    procedure BtnClearClick(Sender: TObject);
    procedure BtnOKClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    fotos: TStrings;
    function GetFotos(): AnsiString;
  end;

implementation

{$R *.DFM}

function TFormCalibration1.GetFotos(): AnsiString;
begin
    Result := fotos.GetText();
end;

procedure TFormCalibration1.BtnLoadClick(Sender: TObject);
var
    i: Longint;
    filename: AnsiString;
begin
    if (not OpenDialog1.Execute()) then begin
        Exit;
    end;

    ListBox1.Items.Clear();
    fotos.Clear();
    for i := 0 to OpenDialog1.Files.Count-1 do begin
        filename := OpenDialog1.Files.Strings[i];
        fotos.Add(filename);
        filename := ExtractFileName(filename);
        ListBox1.Items.Add(filename);
    end;
end;

procedure TFormCalibration1.BtnClearClick(Sender: TObject);
begin
    fotos.Clear();
    ListBox1.Items.Clear();
end;

procedure TFormCalibration1.BtnOKClick(Sender: TObject);
begin
    Self.Tag := 1;
    Self.Close();
end;

procedure TFormCalibration1.Button2Click(Sender: TObject);
begin
    Self.Tag := 0;
    Self.Close();
end;

procedure TFormCalibration1.FormShow(Sender: TObject);
begin
    Self.Tag := 0;
end;

procedure TFormCalibration1.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
    Action := caHide;
end;

procedure TFormCalibration1.FormCreate(Sender: TObject);
begin
    fotos := TStringList.Create();
end;

procedure TFormCalibration1.FormDestroy(Sender: TObject);
begin
    fotos.Free();
end;

end.
