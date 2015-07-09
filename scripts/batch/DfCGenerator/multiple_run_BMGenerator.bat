@echo off
REM -- SELECT ACCORDING FROM AVAILABLE CONFIGURATIONS
REM -- x86_debug
REM -- x86_release
REM -- x64_debug
REM -- x64_release
set CONFIGURATION_NAME=x64_release

REM -- DEFINE PROJECT PATH
set PROJECT_PATH=G:\data\sebastianvilla139\work\fishclef\vs2010\DfCGenerator

REM -- DEFINE EXECUTABLE NAME
set EXEC_NAME=BMGenerator.exe

REM -- DEFINE DATASET PATH
set DATASET_PATH=G:\data\sebastianvilla139\work\fishclef\dataset\videos

REM -- DEFINE BATCH SUPPORT SCRIPTS PATH
set SUPPORT_PATH=..\support

REM -- DEFINE EXECUTABLE PATH
REM -- x86_debug 	-> Debug\EXEC_NAME
REM -- x86_release 	-> Release\EXEC_NAME
REM -- x64_debug 	-> x64\Debug\EXEC_NAME
REM -- x64_release 	-> x64\Release\EXEC_NAME
if %CONFIGURATION_NAME%==x86_debug (
   set EXECUTABLE_PATH=Debug\%EXEC_NAME%
) ELSE IF %CONFIGURATION_NAME%==x86_release (
   set EXECUTABLE_PATH=Release\%EXEC_NAME%
) ELSE IF %CONFIGURATION_NAME%==x64_debug (
   set EXECUTABLE_PATH=x64\Debug\%EXEC_NAME%
) ELSE IF %CONFIGURATION_NAME%==x64_release (
   set EXECUTABLE_PATH=x64\Release\%EXEC_NAME%
)
echo Running for %CONFIGURATION_NAME%...

call "%SUPPORT_PATH%\set_global_env_vars.bat"
call "%SUPPORT_PATH%\set_%CONFIGURATION_NAME%_env_vars.bat"
call "%SUPPORT_PATH%\set_exec_dir_env_vars.bat"

set PATH=%PATH%;%EXEC_DIR_PATH%

echo Using PATH=%PATH%

for %%s in (%DATASET_PATH%\*.flv) do %PROJECT_PATH%\%EXECUTABLE_PATH% %%s  0 1 1