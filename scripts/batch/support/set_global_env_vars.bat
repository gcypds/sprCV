@echo off

REM -- Boost libraries
REM -- Global ENV VARs
set BOOST_BASE_DIR=%CPP_LIBS_DIR%\lib\boost\1_57_0
set BOOST_X86_DIR=%BOOST_DIR%\lib32-msvc-10.0
set BOOST_X64_DIR=%BOOST_DIR%\lib64-msvc-10.0

REM -- OpenCV libraries
REM -- Global ENV VARs
set OPENCV_BASE_DIR=%CPP_LIBS_DIR%\lib\opencv\2_4_10
set OPENCV_X86_DIR=%OPENCV_BASE_DIR%\x86
set OPENCV_X64_DIR=%OPENCV_BASE_DIR%\x64

REM -- Xerces libraries
REM -- Global ENV VARs
set XERCES_X86_DIR=%CPP_LIBS_DIR%\lib\xerces\xerces-c-3.1.1-x86-windows-vc-10.0
set XERCES_X64_DIR=%CPP_LIBS_DIR%\lib\xerces\xerces-c-3.1.1-x86_64-windows-vc-10.0

REM -- Xalan libraries
REM -- Global ENV VARs
set XALAN_X86_DIR=%CPP_LIBS_DIR%\lib\xalan\xalan_c-1.11-x86-windows-VC100
set XALAN_X64_DIR=%CPP_LIBS_DIR%\lib\xalan\xalan_c-1.11-amd64-windows-VC100

REM -- XQilla libraries
REM -- Global ENV VARs
set XQILLA_BASE_DIR=%CPP_LIBS_DIR%\lib\xqilla\XQilla-2.3.0
set XQILLA_X86_DIR=%XQILLA_BASE_DIR%\build\windows\VC8\Win32
set XQILLA_X64_DIR=%XQILLA_BASE_DIR%\build\windows\VC8\x64

REM -- XSD libraries
REM -- Global ENV VARs
set XSD_DIR=%CPP_LIBS_DIR%\lib\xsd\xsd-4.0.0-i686-windows