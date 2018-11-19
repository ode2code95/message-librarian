; Message_Librarian_Setup.nsi
;
; This script will generate an installer and an uninstaller
; for Message Librarian.

;--------------------------------

SetCompressor /FINAL /SOLID lzma

!include "FileFunc.nsh"

!ifdef HAVE_UPX
!packhdr tmp.dat "upx\upx -9 tmp.dat"
!endif

!ifdef NOCOMPRESS
SetCompress off
!endif

!getdllversion "..\..\Deploy\Message_Librarian.exe" Expv_	; Extracts version number from currently deployed build!
!define VERSION "${Expv_1}.${Expv_2}.${Expv_3}"

;--------------------------------

Name "Message Librarian v. ${VERSION}"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
OutFile "..\..\Message Librarian v${VERSION} Setup.exe"

SetDateSave on
SetDatablockOptimize on
CRCCheck force	; forces a self-test for installer integrity
SilentInstall normal
InstallColors /windows ; or try: FF8080 000030
XPStyle on

InstallDir "$PROGRAMFILES\TrueLife Tracks\Message Librarian"
InstallDirRegKey HKU "Software\TrueLife Tracks\Message Librarian" "InstallDir"

CheckBitmap "${NSISDIR}\Contrib\Graphics\Checks\modern.bmp"

LicenseText "Please read the following software license agreement."
LicenseData "..\licdata.rtf"

RequestExecutionLevel admin
ManifestSupportedOS all

;--------------------------------

Page license
; Page components # We have no optional components at this time.
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

Section "" ; This default section will always be executed!

  ; write reg info
  DetailPrint "Writing registry keys . . ."
  WriteRegStr HKU "Software\TrueLife Tracks\Message Librarian" "InstallDir" "$INSTDIR"

  ; write uninstall strings
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "DisplayName" "Message Librarian (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "UninstallString" '"$INSTDIR\messagelib-uninst.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "DisplayIcon" '"$INSTDIR\bin\graphics\MsgLibIco_MultiRes.ico"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "Publisher" "TrueLife Tracks"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "DisplayVersion" "${VERSION}"

  CreateDirectory "$INSTDIR\bin\graphics"	; This recursively creates all non-existent directories, so we make both 'bin' and 'graphics' dirs here.
  SetOutPath $INSTDIR\bin

  File /r /x *.exe ..\..\Deploy\*.*	; Install all files except the EXE
  File /oname=$(^Name).exe ..\..\Deploy\Message_Librarian.exe	; Install the EXE with version-tailored name
  File /oname=graphics\MsgLibIco_MultiRes.ico ..\MsgLibIco_MultiRes.ico

  SetOutPath $INSTDIR
  File ..\License.txt

  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2   ; Finds the size of the installation (with default options).
  IntFmt $3 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian" "EstimatedSize" $3    ; Writes the size to the registry so that the uninstaller entry is accurate.
  IntOp $4 $0 / 1024    ; Yields the number of MBs for a more concise output in the logs.
  DetailPrint "Installed size is $4 MB."

  ; Check for existing installation; create database if it does not exist.
  ReadRegStr $0 HKU "Software\TrueLife Tracks\Message Librarian\paths" "databaseLocation"
  StrCmp $0 "" 0 SkipInitSetup
    DetailPrint "Performing initial database setup . . ."
    ExecWait '"$INSTDIR\bin\$(^Name).exe" -init-setup'
    Goto Continue
  SkipInitSetup:
    DetailPrint 'SKIP: Create message library. One already exists at "$0".'
  Continue:
  WriteUninstaller "messagelib-uninst.exe"

SectionEnd


;--------------------------------

; Uninstaller

UninstallText 'This will completely remove $(^Name) from your computer. Press "Uninstall" to continue.'
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

Section "Uninstall"

  DetailPrint "Removing registry entries . . ."
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MessageLibrarian"

  ;Check for existence of audio message library
  ReadRegStr $1 HKU "Software\TrueLife Tracks\Message Librarian\paths" "databaseLocation"
  StrCmp $1 "" RemoveLibrary_Error
    ;Check for removal of actual audio message library itself!
    MessageBox MB_YESNO|MB_ICONQUESTION 'Do you want to remove the audio message library at "$1" itself?$\nNOTE: This will also delete all the audio files that are part of the library!$\nClick No if you plan to reinstall a newer version.' /SD IDNO IDNO RemoveLibrary_Skip
      DetailPrint "Shredding entire audio message library!"
      RMDir /r "$1"
      Goto RemoveLibrary_Post
  RemoveLibrary_Error:
      ; Print this if we cannot find the library to remove it.
      DetailPrint 'SKIP: Remove message library. Could not find database at "$1".'
	  Goto RemoveLibrary_Post
  RemoveLibrary_Skip:
      ; Print this if user opted not to remove the library.
	  DetailPrint 'SKIP: Remove message library. User selected not to remove it.'
  RemoveLibrary_Post:

  ;Check for removal of registry entries.
  MessageBox MB_YESNO|MB_ICONQUESTION "Do you want to remove the settings for Message Librarian?$\nClick No if you plan to reinstall a newer version." /SD IDNO IDNO RemoveSettings_Skip
    DetailPrint "Removing application settings from system registry . . ."
    DeleteRegKey HKU "Software\TrueLife Tracks\Message Librarian"
    IfErrors 0 RemoveSettings_Skip
      MessageBox MB_OK|MB_ICONEXCLAMATION "Error removing registry entries. Please contact tech support for assistance." IDOK 0
  RemoveSettings_Skip:

  ;Remove the rest of the program
  RMDir /r "$INSTDIR\bin"
  Delete "$INSTDIR\License.txt"
  Delete /REBOOTOK "$INSTDIR\messagelib-uninst.exe"
  Sleep 2000 ; Give system time to catch up with file removal before checking for other files.

  ;Remove the parent directory in $INSTDIR if there are no other programs from TrueLifeTracks installed
  ;This will also protect other files that were not intended to be removed by the uninstaller, in the event that other files are present.
  IfFileExists "$INSTDIR\*.*" 0 NoFilesFound
    MessageBox MB_OK "Note: $INSTDIR will not be removed! Possible cause:$\nOther non-program files are still present." IDOK 0
    Goto CheckParentDir
  NoFilesFound:
    ;Remove $INSTDIR - Get parent directory first and switch to that.
    ${GetParent} "$INSTDIR" $R0
    SetOutPath "$R0"
    RMDir /REBOOTOK "$INSTDIR"

    ;IfFileExists "*.*" CheckRegKeys 0
    ;  RMDir "\" ; Do NOT use the /r switch here; very dangerous!
  CheckParentDir:

  ; CheckRegKeys:

  ; What methods are others using to remove only our files ??? - possible solution - Use another mechanism from IfFileExists to determine the existence of sub files. GetSize with adding file and folder count, then comparing with 0.
  ; Why are our registry keys in HKU not being read?
  ; Add check for existing version / compare version / abort on installed=newer. (in .onInit)

SectionEnd