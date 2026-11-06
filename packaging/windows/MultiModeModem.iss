#ifndef AppVersion
  #error AppVersion is required
#endif
#ifndef PayloadRoot
  #error PayloadRoot is required
#endif
#ifndef OutputRoot
  #error OutputRoot is required
#endif
[Setup]
AppId={{44C5C2FB-3EF8-41FA-84BF-A518655068B2}
AppName=MultiModeModem
AppVersion={#AppVersion}
AppPublisher=Michael Grigutsch, DO3BOX
AppCopyright=Copyright (C) 2026 Papadopol Lucian-Ioan, Michael Grigutsch
AppPublisherURL=https://github.com/mi-gri/MultiModeModem
DefaultDirName={localappdata}\Programs\MultiModeModem
DefaultGroupName=MultiModeModem
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
UsePreviousAppDir=yes
DisableProgramGroupPage=yes
OutputDir={#OutputRoot}
OutputBaseFilename=MultiModeModem-{#AppVersion}-Windows-x64-Setup
SetupIconFile=..\..\icons\multimodemodem.ico
UninstallDisplayIcon={app}\bin\MultiModeModem.exe
LicenseFile={#PayloadRoot}\INSTALLER_LICENSE.txt
Compression=lzma2
SolidCompression=yes
CloseApplications=yes
RestartApplications=no
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#PayloadRoot}\cpu-probe.exe"; Flags: dontcopy
Source: "{#PayloadRoot}\legacy\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: UseLegacy
Source: "{#PayloadRoot}\avx2\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: UseAvx2

[Icons]
Name: "{group}\MultiModeModem"; Filename: "{app}\bin\MultiModeModem.exe"; WorkingDir: "{app}\bin"
Name: "{autodesktop}\MultiModeModem"; Filename: "{app}\bin\MultiModeModem.exe"; WorkingDir: "{app}\bin"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\MultiModeModem.exe"; Description: "{cm:LaunchProgram,MultiModeModem}"; Flags: nowait postinstall skipifsilent

[Code]
var
  Avx2Available: Boolean;

function UseAvx2: Boolean;
begin
  Result := Avx2Available;
end;

function UseLegacy: Boolean;
begin
  Result := not Avx2Available;
end;

procedure InitializeWizard;
var
  ExitCode: Integer;
begin
  Avx2Available := False;
  ExtractTemporaryFile('cpu-probe.exe');
  if Exec(ExpandConstant('{tmp}\cpu-probe.exe'), '', '', SW_HIDE,
          ewWaitUntilTerminated, ExitCode) then
    Avx2Available := ExitCode = 0;
  if Avx2Available then Log('Selected AVX2/FMA build')
  else Log('Selected portable legacy build');
end;
