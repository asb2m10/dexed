;;; Special thanks for Oli for this !!!
[Setup]
AppName=Dexed
AppVersion=0.9.4
DefaultDirName={pf32}\Dexed
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
Name: "vst2_32"; Description: "32-bit VST2 Plugin (.dll)"; Types: full custom;
Name: "vst2_64"; Description: "64-bit VST2 Plugin (.dll)"; Types: full custom;

[Files]
Source: "..\..\..\Builds\VisualStudio2017\Win32\Release\Standalone Plugin\Dexed.exe"; DestDir: "{app}"; Components:app; Flags: ignoreversion recursesubdirs createallsubdirs 
Source: "..\..\..\Builds\VisualStudio2017\Win32\Release\VST\Dexed.dll"; DestDir: {code:GetVST2Dir_32}; Components:vst2_32; Flags: ignoreversion 
Source: "..\..\..\Builds\VisualStudio2017\x64\Release\VST\Dexed.dll"; DestDir: {code:GetVST2Dir_64}; Components:vst2_64; Flags: ignoreversion 
                   
[Icons]
Name: "{group}\Dexed"; Filename: "{app}\Dexed.exe"
Name: "{group}\Un-install Dexed"; Filename: "{app}\unins000.exe"

[code]
var
  OkToCopyLog : Boolean;
  VST2DirPage_32: TInputDirWizardPage;
  VST2DirPage_64: TInputDirWizardPage;

procedure InitializeWizard;
begin
  if IsWin64 then begin
    VST2DirPage_64 := CreateInputDirPage(wpSelectDir,
    'Confirm 64-Bit VST2 Plugin Directory', '',
    'Select the folder in which setup should install the 64-bit VST2 Plugin, then click Next.',
    False, '');
    VST2DirPage_64.Add('');
    VST2DirPage_64.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\VST,VSTPluginsPath|{pf}\Steinberg\VSTPlugins}\');

    VST2DirPage_32 := CreateInputDirPage(wpSelectDir,
      'Confirm 32-Bit VST2 Plugin Directory', '',
      'Select the folder in which setup should install the 32-bit VST2 Plugin, then click Next.',
      False, '');
    VST2DirPage_32.Add('');
    VST2DirPage_32.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\WOW6432NODE\VST,VSTPluginsPath|{pf32}\Steinberg\VSTPlugins}\');
  end else begin
    VST2DirPage_32 := CreateInputDirPage(wpSelectDir,
      'Confirm 32-Bit VST2 Plugin Directory', '',
      'Select the folder in which setup should install the 32-bit VST2 Plugin, then click Next.',
      False, '');
    VST2DirPage_32.Add('');
    VST2DirPage_32.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\VST,VSTPluginsPath|{pf}\Steinberg\VSTPlugins}\');
  end;
end;

function GetVST2Dir_32(Param: String): String;
begin
  Result := VST2DirPage_32.Values[0]
end;

function GetVST2Dir_64(Param: String): String;
begin
  Result := VST2DirPage_64.Values[0]
end;

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
