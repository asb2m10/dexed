[Setup]
AppName=Dexed
AppVersion=0.9.5
DefaultDirName={commonpf64}\Dexed
DefaultGroupName=Dexed
Compression=lzma2
SolidCompression=yes
OutputDir=.\
OutputBaseFilename=DexedInstaller
LicenseFile=..\..\..\LICENSE
SetupLogging=yes
ArchitecturesInstallIn64BitMode=x64
AppPublisher=Digital Suburban

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "app"; Description: "Standalone application (.exe)"; Types: full custom;
Name: "vst3_64"; Description: "64-bit VST3 Plugin (.dll)"; Types: full custom;

[Files]
Source: "Dexed.exe"; DestDir: "{app}"; Components:app; Flags: ignoreversion recursesubdirs createallsubdirs 
Source: "Dexed.vst3"; DestDir: "{commoncf64}\VST3"; Components:vst3_64; Flags: ignoreversion 
                   
[Icons]
Name: "{group}\Dexed"; Filename: "{app}\Dexed.exe"
Name: "{group}\Un-install Dexed"; Filename: "{app}\unins000.exe"

[code]
var
  OkToCopyLog : Boolean;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssDone then
    OkToCopyLog := True;
end;

procedure DeinitializeSetup();
begin
  if OkToCopyLog then
    FileCopy (ExpandConstant ('{log}'), ExpandConstant ('{app}\InstallationLogFile.log'), FALSE);
  RestartReplace (ExpandConstant ('{log}'), '');
end;

[UninstallDelete]
Type: files; Name: "{app}\InstallationLogFile.log"
