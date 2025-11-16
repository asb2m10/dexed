#ifndef Publisher
#define Publisher "Digital Suburban"
#endif

#ifndef Version
#define Version "0.0.0"
#endif

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Setup]
OutputBaseFilename={#NameCondensed}-{#Version}-win
AppName={#Name}
AppPublisher={#Publisher}
AppVerName={#Name}
AppVersion={#Version}
ArchitecturesInstallIn64BitMode=x64
CloseApplicationsFilter=*.exe,*.vst3
DefaultDirName={autopf}\{#Name}\
DefaultGroupName={#Name}
DisableDirPage=no
DisableProgramGroupPage=yes
#ifdef License
LicenseFile={#License}
#endif
#ifdef Icon
SetupIconFile={#Icon}
#endif
SetupLogging=yes
SolidCompression=yes
Compression=zip
UninstallDisplayIcon={uninstallexe}
UninstallFilesDir={autoappdata}\{#Name}\uninstall
WizardStyle=modern
WizardSizePercent=100

; Enable non-elevated install. Disabled until we resolve issues with DAWs.
; Code section below will convert AppData location from Roaming to Local
; PrivilegesRequired=lowest
; PrivilegesRequiredOverridesAllowed=dialog

[Types]
Name: "full"; Description: "Full"
Name: "custom"; Description: "Custom"; Flags: iscustom
#ifdef CLAP
Name: "clap"; Description: "CLAP"
#endif
#ifdef VST3
Name: "vst3"; Description: "VST3"
#endif
#ifdef SA
Name: "standalone"; Description: "Standalone"
#endif

[Components]
#ifdef CLAP
Name: "CLAP"; Description: "CLAP"; Types: full custom clap; Flags: checkablealone
#endif

#ifdef VST3
Name: "VST3"; Description: "VST3"; Types: full custom vst3; Flags: checkablealone
#endif

#ifdef SA
Name: "SA"; Description: "Standalone"; Types: full custom standalone; Flags: checkablealone
#endif

[Files]
#ifdef CLAP
Source: "{#StagedAssets}\{#Name}.clap"; DestDir: "{autocf}\CLAP\"; Components: CLAP; Flags: ignoreversion
#endif

#ifdef VST3
Source: "{#StagedAssets}\{#Name}.vst3\*"; DestDir: "{autocf}\VST3\{#Name}.vst3\"; Components: VST3; Flags: ignoreversion recursesubdirs
#endif

#ifdef SA
Source: "{#StagedAssets}\{#Name}.exe"; DestDir: "{app}"; Components: SA; Flags: ignoreversion
#endif

#define FORMATS ""

#ifdef Data
[Components]
#ifdef CLAP
#define FORMATS " clap"
#endif
#ifdef VST3
#define FORMATS FORMATS + " vst3"
#endif
#ifdef SA
#define FORMATS FORMATS + " standalone"
#endif

Name: "Data"; Description: "Data files"; Types: full custom{#FORMATS}; Flags: fixed disablenouninstallwarning
[Files]
Source: "{#Data}\*"; DestDir: "{code:get_data_path}"; Flags: ignoreversion recursesubdirs
#endif

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#Name}.exe"; Flags: createonlyiffileexists

#ifdef VST3
[InstallDelete]
Type: files; Name: "{autocf}\VST3\{#Name}.vst3"
#endif VST3

[Code]
function get_data_path(Param: string): string;
begin
  if IsAdminInstallMode then
    // System-wide → ProgramData
    Result := ExpandConstant('{commonappdata}')
  else
    // Per-user → LocalAppData
    Result := ExpandConstant('{localappdata}');
end;
