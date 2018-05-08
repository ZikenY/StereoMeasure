object FormCalibration2: TFormCalibration2
  Left = 278
  Top = 147
  Width = 507
  Height = 439
  Caption = 'Calibrate left-right-separated fotos'
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
  object Label2: TLabel
    Left = 16
    Top = 330
    Width = 105
    Height = 16
    Caption = 'chessboard width'
  end
  object Label3: TLabel
    Left = 8
    Top = 362
    Width = 111
    Height = 16
    Caption = 'chessboard height'
  end
  object Label4: TLabel
    Left = 200
    Top = 330
    Width = 108
    Height = 16
    Caption = 'chess square size'
  end
  object Label5: TLabel
    Left = 378
    Top = 331
    Width = 18
    Height = 16
    Caption = 'cm'
  end
  object Label6: TLabel
    Left = 224
    Top = 362
    Width = 80
    Height = 16
    Caption = 'ccd pixel size'
  end
  object Label7: TLabel
    Left = 379
    Top = 362
    Width = 18
    Height = 16
    Caption = 'um'
  end
  object chessboard_width: TSpinEdit
    Left = 128
    Top = 328
    Width = 65
    Height = 26
    MaxValue = 0
    MinValue = 0
    TabOrder = 0
    Value = 9
  end
  object chessboard_height: TSpinEdit
    Left = 128
    Top = 360
    Width = 65
    Height = 26
    MaxValue = 0
    MinValue = 0
    TabOrder = 1
    Value = 6
  end
  object BtnOK: TButton
    Left = 400
    Top = 56
    Width = 75
    Height = 25
    Caption = 'calibrate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = BtnOKClick
  end
  object Button2: TButton
    Left = 400
    Top = 88
    Width = 75
    Height = 25
    Caption = 'cancel'
    TabOrder = 5
    OnClick = Button2Click
  end
  object BtnLoadLeft: TButton
    Left = 16
    Top = 16
    Width = 161
    Height = 25
    Caption = 'fotos from left camera:'
    TabOrder = 6
    OnClick = BtnLoadLeftClick
  end
  object BtnClear: TButton
    Left = 400
    Top = 16
    Width = 75
    Height = 25
    Caption = 'clear fotos'
    TabOrder = 7
    OnClick = BtnClearClick
  end
  object LBLeft: TListBox
    Left = 16
    Top = 48
    Width = 161
    Height = 273
    ItemHeight = 16
    TabOrder = 8
  end
  object LBRight: TListBox
    Left = 216
    Top = 48
    Width = 161
    Height = 273
    ItemHeight = 16
    TabOrder = 9
  end
  object BtnLoadRight: TButton
    Left = 216
    Top = 16
    Width = 161
    Height = 25
    Caption = 'fotos from right camera:'
    TabOrder = 10
    OnClick = BtnLoadRightClick
  end
  object squaresize: TEdit
    Left = 312
    Top = 328
    Width = 63
    Height = 24
    TabOrder = 2
    Text = '9.0'
  end
  object ccdpixelsize: TEdit
    Left = 312
    Top = 360
    Width = 63
    Height = 24
    TabOrder = 3
    Text = '4.4'
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '.jpg'
    Filter = 'calibrating foto(*.jpg)|*.jpg'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofFileMustExist, ofEnableSizing]
    Left = 176
    Top = 144
  end
end
