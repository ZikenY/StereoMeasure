unit UnitPlacemark;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons;

type
  TFormPlacemark = class(TForm)
    Edit1: TEdit;
    CBGetDistance: TCheckBox;
    SBMatch: TSpeedButton;
    SBUnCancelTrack: TSpeedButton;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure SBMatchClick(Sender: TObject);
    procedure SBUnCancelTrackClick(Sender: TObject);
    procedure CBGetDistanceClick(Sender: TObject);
    procedure Edit1KeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormPlacemark: TFormPlacemark;

implementation

uses
    UnitMainFrm;
{$R *.DFM}

procedure TFormPlacemark.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
    Action := caHide;
end;

procedure TFormPlacemark.SBMatchClick(Sender: TObject);
begin
    FormMain.SBMatchClick(Sender);
    FormMain.SetFocus();
end;

procedure TFormPlacemark.SBUnCancelTrackClick(Sender: TObject);
begin
    FormMain.SBUnCancelTrackClick(Sender);
    FormMain.SetFocus();
end;

procedure TFormPlacemark.CBGetDistanceClick(Sender: TObject);
begin
    FormMain.SetFocus();
end;

procedure TFormPlacemark.Edit1KeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
    if (Key = 13) then begin
        FormMain.SetFocus();
    end;
end;

end.
