@echo off
setlocal enableextensions enabledelayedexpansion

REM -- Define script flags
SET CREATE_STIP_INDEX=1
SET CREATE_HLS_FILES=1

REM -- Define dataset path
set DATASET_PATH=Z:\master\survmantics\dataset\processed

REM -- Define STIP file name pattern
set "STIP_FILENAME_PATTERN=video_bg_stip_"
	
REM -- Define concatenation path
set CONCATENATION_PATH=Z:\master\survmantics\dataset\concatenated

REM -- Define HLS path
set HLS_PATH=Z:\master\survmantics\dataset\hls

if %CREATE_STIP_INDEX%==1 (
	echo Creating full STIP index...		

	REM -- Create output directories if necessary
	if not exist "%CONCATENATION_PATH%" mkdir %CONCATENATION_PATH%

	REM -- CYGWIN equivalent: find `pwd` -name 'roi_stip_*' > hls_file_index.txt
	break>%CONCATENATION_PATH%\hls_file_index.txt

	for /R "%DATASET_PATH%" %%# in (*.txt) do (	
		echo %%~nx# | find "%STIP_FILENAME_PATTERN%" 1>NUL && (
			echo %%~#
			echo %%~# >> %CONCATENATION_PATH%\hls_file_index.txt
			REM -- echo full path: %%~#
			REM -- echo file name: %%~nx#
			REM -- echo directory: %%~p#
		)
	)
	
	echo:
	echo full STIP index created!
	echo:

	echo Creating DISTRACTED STIP index...
	REM -- CYGWIN equivalent: grep 'DISTRACTED' hls_file_index.txt > DISTRACTED_hls_index.txt
	findstr "DISTRACTED" %CONCATENATION_PATH%\hls_file_index.txt > %CONCATENATION_PATH%\DISTRACTED_hls_index.txt

	echo Creating EXPLORING STIP index...
	REM -- CYGWIN equivalent: grep 'EXPLORING' hls_file_index.txt > EXPLORING_hls_index.txt
	findstr "EXPLORING" %CONCATENATION_PATH%\hls_file_index.txt > %CONCATENATION_PATH%\EXPLORING_hls_index.txt

	echo Creating DISORIENTED STIP index...
	REM -- CYGWIN equivalent: grep 'DISORIENTED' hls_file_index.txt > DISORIENTED_hls_index.txt
	findstr "DISORIENTED" %CONCATENATION_PATH%\hls_file_index.txt > %CONCATENATION_PATH%\DISORIENTED_hls_index.txt

	echo Creating INTERESTED STIP index...
	REM -- CYGWIN equivalent: grep 'INTERESTED' hls_file_index.txt > INTERESTED_hls_index.txt
	findstr "INTERESTED" %CONCATENATION_PATH%\hls_file_index.txt > %CONCATENATION_PATH%\INTERESTED_hls_index.txt

	REM -- CYGWIN additional: 
	REM -- sed -i -e 's/\/cygdrive\/z/Z\:/g' *_hls_index.txt
	REM -- sed -i -e 's/\//\\/g' *_hls_index.txt

	echo:
	echo STIP indexes created!
	echo:
)

if %CREATE_HLS_FILES%==1 (
	echo:
	echo Creating High Level Semantic files...

	if not exist "%HLS_PATH%" mkdir %HLS_PATH%

	echo:
	echo Concatenating DISTRACTED High Level Semantic...
	break>%HLS_PATH%\DISTRACTED_stip_hls.txt	
	for /F "tokens=*" %%A in (%CONCATENATION_PATH%\DISTRACTED_hls_index.txt) do (
		if exist "%%A" (
			echo Merging %%A...
			type %%A >> %HLS_PATH%\DISTRACTED_stip_hls.txt
		)
	)

	echo:
	echo Concatenating EXPLORING High Level Semantic...
	break>%HLS_PATH%\EXPLORING_stip_hls.txt
	for /F "tokens=*" %%A in (%CONCATENATION_PATH%\EXPLORING_hls_index.txt) do (
		if exist "%%A" (
			echo Merging %%A...
			type %%A >> %HLS_PATH%\EXPLORING_stip_hls.txt
		)
	)

	echo:
	echo Concatenating DISORIENTED High Level Semantic...
	break>%HLS_PATH%\DISORIENTED_stip_hls.txt
	for /F "tokens=*" %%A in (%CONCATENATION_PATH%\DISORIENTED_hls_index.txt) do (
		if exist "%%A" (
			echo Merging %%A...
			type %%A >> %HLS_PATH%\DISORIENTED_stip_hls.txt
		)
	)

	echo:
	echo Concatenating INTERESTED High Level Semantic...
	break>%HLS_PATH%\INTERESTED_stip_hls.txt
	for /F "tokens=*" %%A in (%CONCATENATION_PATH%\INTERESTED_hls_index.txt) do (
		if exist "%%A" (
			echo Merging %%A...
			type %%A >> %HLS_PATH%\INTERESTED_stip_hls.txt
		)
	)

	echo:
	echo HLS concatenation finished!
	echo:
)

endlocal
pause&exit