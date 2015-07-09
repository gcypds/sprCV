@echo off
REM -- SELECT ACCORDING FROM AVAILABLE CONFIGURATIONS
REM -- x86_debug
REM -- x86_release
REM -- x64_debug
REM -- x64_release
set CONFIGURATION_NAME=x64_release

REM -- DEFINE PROJECT PATH
set PROJECT_PATH=C:\CPP_LIBS\survmantics\vs\gta

REM -- DEFINE DATASET PATH
set DATASET_PATH=Z:\master\survmantics\dataset\processed

REM -- DEFINE BATCH SUPPORT SCRIPTS PATH
set SUPPORT_PATH=..\support

REM -- DEFINE EXECUTABLE PATH
REM -- x86_debug 	-> Debug\hagta.exe
REM -- x86_release 	-> Release\hagta.exe
REM -- x64_debug 	-> x64\Debug\hagta.exe
REM -- x64_release 	-> x64\Release\hagta.exe
if %CONFIGURATION_NAME%==x86_debug (
   set EXECUTABLE_PATH=Debug\hagta.exe   
) ELSE IF %CONFIGURATION_NAME%==x86_release (
   set EXECUTABLE_PATH=Release\hagta.exe   
) ELSE IF %CONFIGURATION_NAME%==x64_debug (
   set EXECUTABLE_PATH=x64\Debug\hagta.exe   
) ELSE IF %CONFIGURATION_NAME%==x64_release (
   set EXECUTABLE_PATH=x64\Release\hagta.exe   
)
echo Running for %CONFIGURATION_NAME%...

call "%SUPPORT_PATH%\set_global_env_vars.bat"
call "%SUPPORT_PATH%\set_%CONFIGURATION_NAME%_env_vars.bat"
call "%SUPPORT_PATH%\set_exec_dir_env_vars.bat"

set PATH=%PATH%;%EXEC_DIR_PATH%

echo Using PATH=%PATH%

for /D %%s in (%DATASET_PATH%\*) do %PROJECT_PATH%\%EXECUTABLE_PATH% %%s\group.xml %%s 0 0 0 0 1