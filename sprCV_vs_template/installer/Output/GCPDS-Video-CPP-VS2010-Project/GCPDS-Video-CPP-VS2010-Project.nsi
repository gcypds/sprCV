############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "GCPDS-Video-CPP-VS2010-Project"
!define COMP_NAME "GCPDS"
!define WEB_SITE "sebastianvilla139@gmail.com"
!define VERSION "00.00.00.00"
!define COPYRIGHT "Sebastian Villa  © 2015"
!define DESCRIPTION "GCPDS-Video-CPP-VS2010-Project"
!define INSTALLER_NAME "G:\data\sebastianvilla139\work\nsis\Nsisqssg\Output\GCPDS-Video-CPP-VS2010-Project\v9_GCPDS-Video-CPP-VS2010-Project.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor LZMA
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${UNINSTALL_PATH}" "UninstallString"
InstallDir "$DOCUMENTS\Visual Studio 2010"
######################################################################
 
!include "MUI.nsh"
!include "TextReplace.nsh"
 
!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram

#MessageBox MB_OK "DOCUMENTS: $DOCUMENTS"

#MessageBox MB_OK "INSTDIR: $INSTDIR"

!define INSTALL_DIR "$DOCUMENTS\Visual Studio 2010";

#MessageBox MB_OK "INSTALL_DIR: ${INSTALL_DIR}"

!define USER_PROJECT_NAME "GCPDS-Video-CPP-VS2010-Project_test_2"
#MessageBox MB_OK "USER_PROJECT_NAME: ${USER_PROJECT_NAME}"

!define USER_INSTALL_PATH "${INSTALL_DIR}\Wizards\${USER_PROJECT_NAME}"
#MessageBox MB_OK "USER_INSTALL_PATH: ${USER_INSTALL_PATH}"

!define USER_PROJECT_PATH "${INSTALL_DIR}\WizardsContent\${USER_PROJECT_NAME}"
#MessageBox MB_OK "USER_PROJECT_PATH: ${USER_PROJECT_PATH}"

!define USER_UNINSTALL_PATH "${INSTALL_DIR}\WizardsContent"

${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "${USER_INSTALL_PATH}"
#MessageBox MB_OK "${USER_INSTALL_PATH}"

File /oname=${USER_PROJECT_NAME}.ico "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Config\TEMPLATE.ico"

${textreplace::ReplaceInFile} "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Config\TEMPLATE.vsdir" "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsdir" "@@WIZARD_NAME@@" "${USER_PROJECT_NAME}" "" $0
${textreplace::ReplaceInFile} "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Config\TEMPLATE.vsz" "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsz" "@@WIZARD_NAME@@" "${USER_PROJECT_NAME}" "" $0
${textreplace::ReplaceInFile} "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsz" "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsz" "@@WIZARD_PATH@@" "${USER_PROJECT_PATH}" "" $0

#MessageBox MB_OK "textreplace::ReplaceInFile$\n$$0={$0} Output file: ${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsdir"

${textreplace::Unload}

SetOutPath "${USER_PROJECT_PATH}"
SetOutPath "${USER_PROJECT_PATH}\Templates\1033"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Templates\1033\ReadMe.txt"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Templates\1033\Sample.txt"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Templates\1033\Templates.inf"
SetOutPath "${USER_PROJECT_PATH}\Scripts\1033"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Scripts\1033\default.js"
SetOutPath "${USER_PROJECT_PATH}\Images"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Images\GCPDS-Video-CPP-VS2010-Project.gif"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\Images\GCPDS-Video-CPP-VS2010-Project_Background.gif"
SetOutPath "${USER_PROJECT_PATH}\HTML\1033"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\HTML\1033\default.htm"
SetOutPath "${USER_PROJECT_PATH}\1033"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\1033\NewStyles.css"
SetOutPath "${USER_PROJECT_PATH}\1033\Images"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\1033\Images\DottedHori.gif"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\1033\Images\DottedVert.gif"
File "G:\data\sebastianvilla139\work\scripts\install_GCPDS-Video_CPP_VS2010-Project-Wizard\Content\Visual Studio 2010\WizardsContent\TEMPLATE\1033\Images\spacer.gif"

SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "${USER_UNINSTALL_PATH}"
WriteUninstaller "${USER_UNINSTALL_PATH}\uninstall_${USER_PROJECT_NAME}.exe"

WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "${USER_PROJECT_PATH}\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.ico"
Delete "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsdir"
Delete "${USER_INSTALL_PATH}\${USER_PROJECT_NAME}.vsz"
RmDir "${USER_INSTALL_PATH}"

Delete "${USER_PROJECT_PATH}\Templates\1033\ReadMe.txt"
Delete "${USER_PROJECT_PATH}\Templates\1033\Sample.txt"
Delete "${USER_PROJECT_PATH}\Templates\1033\Templates.inf"
Delete "${USER_PROJECT_PATH}\Scripts\1033\default.js"
Delete "${USER_PROJECT_PATH}\Images\GCPDS-Video-CPP-VS2010-Project.gif"
Delete "${USER_PROJECT_PATH}\Images\GCPDS-Video-CPP-VS2010-Project_Background.gif"
Delete "${USER_PROJECT_PATH}\HTML\1033\default.htm"
Delete "${USER_PROJECT_PATH}\1033\NewStyles.css"
Delete "${USER_PROJECT_PATH}\1033\Images\DottedHori.gif"
Delete "${USER_PROJECT_PATH}\1033\Images\DottedVert.gif"
Delete "${USER_PROJECT_PATH}\1033\Images\spacer.gif"
 
RmDir "${USER_PROJECT_PATH}\1033\Images"
RmDir "${USER_PROJECT_PATH}\1033"
RmDir "${USER_PROJECT_PATH}\HTML\1033"
RmDir "${USER_PROJECT_PATH}\HTML"
RmDir "${USER_PROJECT_PATH}\Images"
RmDir "${USER_PROJECT_PATH}\Scripts\1033"
RmDir "${USER_PROJECT_PATH}\Scripts"
RmDir "${USER_PROJECT_PATH}\Templates\1033"
RmDir "${USER_PROJECT_PATH}\Templates"
RmDir "${USER_PROJECT_PATH}"

Delete "${USER_UNINSTALL_PATH}\uninstall_${USER_PROJECT_NAME}.exe"

DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################