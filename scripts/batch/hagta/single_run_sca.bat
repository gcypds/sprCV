@echo off
REM -- SELECT ACCORDING FROM AVAILABLE CONFIGURATIONS
REM -- x86_debug
REM -- x86_release
REM -- x64_debug
REM -- x64_release
set CONFIGURATION_NAME=x64_debug

REM -- DEFINE PROJECT PATH
set PROJECT_PATH=G:\data\sebastianvilla139\work\projects\survmantics\github\sca

REM -- DEFINE DATASET PATH
set DATASET_PATH=Z:\master\survmantics\dataset

REM -- DEFINE EXECUTABLE PATH
REM -- x86_debug 	-> Debug\sca.exe
REM -- x86_release 	-> Release\sca.exe
REM -- x64_debug 	-> x64\Debug\sca.exe
REM -- x64_release 	-> x64\Release\sca.exe
if %CONFIGURATION_NAME%==x86_debug (
   set EXECUTABLE_PATH=Debug\sca.exe
) ELSE IF %CONFIGURATION_NAME%==x86_release (
   set EXECUTABLE_PATH=Release\sca.exe
) ELSE IF %CONFIGURATION_NAME%==x64_debug (
   set EXECUTABLE_PATH=x64\Debug\sca.exe
) ELSE IF %CONFIGURATION_NAME%==x64_release (
   set EXECUTABLE_PATH=x64\Release\sca.exe
)

call "set_global_env_vars.bat"
call "set_%CONFIGURATION_NAME%_env_vars.bat"
call "set_exec_dir_env_vars.bat"

set PATH=%PATH%;%EXEC_DIR_PATH%

echo %PATH%

%PROJECT_PATH%\%EXECUTABLE_PATH% %DATASET_PATH%\%1\group.xml %DATASET_PATH%\%1 0 0 0 0 1