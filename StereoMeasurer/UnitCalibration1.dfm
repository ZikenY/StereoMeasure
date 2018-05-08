object FormCalibration1: TFormCalibration1
  Left = 256
  Top = 130
  BorderStyle = bsToolWindow
  Caption = 'Calibrate left-right-mixed fotos'
  ClientHeight = 370
  ClientWidth = 304
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 97
    Height = 16
    Caption = 'calibrating fotos:'
  end
  object Label2: TLabel
    Left = 184
    Top = 8
    Width = 105
    Height = 16
    Caption = 'chessboard width'
  end
  object Label3: TLabel
    Left = 184
    Top = 56
    Width = 111
    Height = 16
    Caption = 'chessboard height'
  end
  object Label4: TLabel
    Left = 184
    Top = 104
    Width = 108
    Height = 16
    Caption = 'chess square size'
  end
  object Label5: TLabel
    Left = 275
    Top = 124
    Width = 18
    Height = 16
    Caption = 'cm'
  end
  object Label6: TLabel
    Left = 200
    Top = 150
    Width = 80
    Height = 16
    Caption = 'ccd pixel size'
  end
  object Label7: TLabel
    Left = 275
    Top = 169
    Width = 18
    Height = 16
    Caption = 'um'
  end
  object ListBox1: TListBox
    Left = 16
    Top = 24
    Width = 161
    Height = 329
    ItemHeight = 16
    TabOrder = 0
  end
  object chessboard_width: TSpinEdit
    Left = 208
    Top = 24
    Width = 65
    Height = 26
    MaxValue = 0
    MinValue = 0
    TabOrder = 1
    Value = 9
  end
  object chessboard_height: TSpinEdit
    Left = 208
    Top = 72
    Width = 65
    Height = 26
    MaxValue = 0
    MinValue = 0
    TabOrder = 2
    Value = 6
  end
  object BtnOK: TButton
    Left = 200
    Top = 280
    Width = 75
    Height = 25
    Caption = 'calibrate'
    TabOrder = 5
    OnClick = BtnOKClick
  end
  object Button2: TButton
    Left = 200
    Top = 312
    Width = 75
    Height = 25
    Caption = 'cancel'
    TabOrder = 6
    OnClick = Button2Click
  end
  object BtnLoad: TButton
    Left = 200
    Top = 200
    Width = 75
    Height = 25
    Caption = 'load fotos'
    TabOrder = 7
    OnClick = BtnLoadClick
  end
  object BtnClear: TButton
    Left = 200
    Top = 232
    Width = 75
    Height = 25
    Caption = 'clear fotos'
    TabOrder = 8
    OnClick = BtnClearClick
  end
  object squaresize: TEdit
    Left = 208
    Top = 121
    Width = 63
    Height = 24
    TabOrder = 3
    Text = '9.0'
  end
  object ccdpixelsize: TEdit
    Left = 208
    Top = 167
    Width = 63
    Height = 24
    TabOrder = 4
    Text = '4.4'
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '.jpg'
    Filter = 'calibrating foto(*.jpg)|*.jpg'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofFileMustExist, ofEnableSizing]
    Left = 152
    Top = 144
  end
end
