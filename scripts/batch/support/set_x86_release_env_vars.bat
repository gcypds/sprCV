@echo off

REM -- Per platform ENV VARs
REM -- x86
set BOOST_DIR=%BOOST_X86_DIR%
set OPENCV_DIR=%OPENCV_X86_DIR%
set XERCES_DIR=%XERCES_X86_DIR%
set XALAN_DIR=%XALAN_X86_DIR%

REM -- x86 Release
set XQILLA_CONFIGURATION=Release
set XQILLA_DIR=%XQILLA_X86_DIR%
set XQILLA_EXEC_DIR=%XQILLA_DIR%\%XQILLA_CONFIGURATION%