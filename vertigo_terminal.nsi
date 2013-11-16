; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "Vertigo terminal"

; The file to write
OutFile "v_terminal.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\Vertigo terminal"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Vertigo_terminal" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Vertigo terminal"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "vertigo_terminal.exe"
  File "*.dll"
  File /r "settings"
  File /r "js"

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Vertigo_terminal "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vertigo_terminal" "DisplayName" "Vertigo Terminal"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vertigo_terminal" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vertigo_terminal" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vertigo_terminal" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Verigo Terminal"
  CreateShortCut "$SMPROGRAMS\Verigo Terminal\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Verigo Terminal\Terminal.lnk" "$INSTDIR\vertigo_terminal.exe" "" "$INSTDIR\vertigo_terminal.exe" 0
  CreateShortCut "$SMPROGRAMS\Verigo Terminal\settings.lnk" "$INSTDIR\settings\settings.ini" "" "$INSTDIR\settings\settings.ini" 0  
  CreateShortCut "$SMPROGRAMS\Verigo Terminal\drivers.lnk" "$INSTDIR\js\drivers.js" "" "$INSTDIR\js\drivers.js" 0  
SectionEnd

Section "Run on windows startup"
  CreateShortCut "$SMSTARTUP\Vertigo terminal.lnk" "$INSTDIR\vertigo_terminal.exe" "" "$INSTDIR\vertigo_terminal.exe" 0
SectionEnd


;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vertigo_terminal"
  DeleteRegKey HKLM SOFTWARE\Vertigo_terminal

  ; Remove files and uninstaller
  Delete $INSTDIR\*

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Verigo Terminal\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Verigo Terminal"
  RMDir "$INSTDIR"
  Delete "$SMSTARTUP\Vertigo terminal.lnk"

SectionEnd
