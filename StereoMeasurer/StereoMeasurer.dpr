program StereoMeasurer;

uses
  Forms,
  UnitMainFrm in 'UnitMainFrm.pas' {FormMain},
  UnitStereoMeasure in 'UnitStereoMeasure.pas',
  UnitFotoSpace in 'UnitFotoSpace.pas',
  UnitPlacemark in 'UnitPlacemark.pas' {FormPlacemark},
  UnitCalibration1 in 'UnitCalibration1.pas' {FormCalibration1},
  UnitCalibration2 in 'UnitCalibration2.pas' {FormCalibration2};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TFormMain, FormMain);
  Application.Run;
end.
