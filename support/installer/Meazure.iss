; Inno Setup Script for Meazure
;
; Copyright 2001, 2004, 2011 C Thing Software

[Setup]
AppId=Meazure
AppName=Meazure
AppVerName=Meazure 2.0.1
AppVersion=2.0.1
AppMutex=MeazureInstallerMutex
OutputDir=..\..\dist
OutputBaseFilename=meazr201
AppPublisher=C Thing Software
AppPublisherURL=http://www.cthing.com
AppSupportURL=http://www.cthing.com
AppUpdatesURL=http://www.cthing.com
AppCopyright=Copyright 2001, 2004, 2011 C Thing Software
DefaultDirName={pf}\Meazure
DefaultGroupName=Meazure
LicenseFile=..\..\LICENSE
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon";     Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4; Flags: unchecked

[Types]
Name: "complete"; Description: "Complete installation"
Name: "compact";  Description: "Compact installation"
Name: "custom";   Description: "Custom installation"; Flags: iscustom

[Components]
Name: "core";     Description: "Program Files";   Types: complete compact custom; Flags: fixed
Name: "help";     Description: "Help Files";      Types: complete custom
Name: "profiles"; Description: "Sample Profiles"; Types: complete custom

[Dirs]
Name: "{app}\Profiles"; Components: core

[Files]
; Visual C++ 6.0 + MFC redistributable system files
Source: "..\..\build-nmake\Release\mfc*.dll";             DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\..\build-nmake\Release\msvc*.dll";            DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\..\build-nmake\Release\Microsoft.*.manifest"; DestDir: "{app}"; Components: core; Flags: ignoreversion skipifsourcedoesntexist
; HTML Help support
Source: "..\redist\hhupd.exe"; DestDir: "{tmp}"; Components: core; Check: Runhhupd
; Core files
Source: "..\..\build-nmake\Release\Meazure.exe";  DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\..\build-nmake\Release\Meazure.chm";  DestDir: "{app}"; Components: help; Flags: ignoreversion
Source: "..\..\build-nmake\Release\Hooks.dll";    DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\..\build-nmake\Release\libexpat.dll"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\..\LICENSE";                          DestDir: "{app}"; Components: core; Flags: ignoreversion
source: "..\..\COPYING";                          DestDir: "{app}"; COmponents: core; Flags: ignoreversion
Source: "..\..\Readme.txt";                       DestDir: "{app}"; Components: core; Flags: ignoreversion isreadme
; DTD files
Source: "..\dtd\PositionLog1.dtd";   DestDir: "{app}\dtd"; Components: core; Flags: ignoreversion
; Sample profiles
Source: "..\profiles\1024x768.mea";  DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\1280x1024.mea"; DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\640x480.mea";   DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\800x600.mea";   DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\NTSC.mea";      DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\PAL.mea";       DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\SVGA.mea";      DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\SXGA.mea";      DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\VGA.mea";       DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly
Source: "..\profiles\XGA.mea";       DestDir: "{app}\Profiles"; Components: profiles; Flags: ignoreversion overwritereadonly uninsremovereadonly; Attribs: readonly

[InstallDelete]
Type: files; Name: "{app}\xmlparse.dll"
Type: files; Name: "{app}\xmltok.dll"

[Icons]
Name: "{group}\Meazure";           Filename: "{app}\Meazure.exe"; Components: core
Name: "{group}\Meazure Help";      Filename: "{app}\Meazure.chm"; Components: help
Name: "{group}\Meazure Readme";    Filename: "{app}\Readme.txt";  Components: core
Name: "{group}\Meazure License";   Filename: "{app}\LICENSE"; Components: core
Name: "{group}\Uninstall Meazure"; Filename: {uninstallexe}; Components: core
Name: "{userdesktop}\Meazure";     Filename: "{app}\Meazure.exe"; MinVersion: 4,4; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Meazure"; Filename: "{app}\Meazure.exe"; MinVersion: 6,0; Tasks: quicklaunchicon

[Registry]
; Profile file type
Root: HKCR; Subkey: ".mea"; ValueType: string; ValueData: "Meazure.Profile"; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCR; Subkey: "Meazure.Profile"; ValueType: string; ValueData: "Meazure Profile File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Profile\DefaultIcon"; ValueType: string; ValueData: "{app}\Meazure.exe,0"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Profile\shell\open"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Profile\shell\open\command"; ValueType: string; ValueData: """{app}\Meazure.exe"" ""%1"""; Flags: uninsdeletekey
; Position log file type
Root: HKCR; Subkey: ".mpl"; ValueType: string; ValueData: "Meazure.Positions"; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCR; Subkey: "Meazure.Positions"; ValueType: string; ValueData: "Meazure Position Log File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Positions\DefaultIcon"; ValueType: string; ValueData: "{app}\Meazure.exe,1"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Positions\shell\open"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Meazure.Positions\shell\open\command"; ValueType: string; ValueData: """{app}\Meazure.exe"" ""%1"""; Flags: uninsdeletekey

[Run]
Filename: "{tmp}\hhupd.exe /q:a /r:n"; Check: Runhhupd
Filename: "{app}\Meazure.exe"; Description: "Launch Meazure"; Flags: nowait postinstall skipifsilent

[Code]
program Setup;

function Runhhupd(): Boolean;
var
  S: String;
begin
  RegQueryStringValue(HKCR, '.chm', '', S);
  Result := (S = '');
end;

begin
end.

