@echo off
setlocal enableextensions enabledelayedexpansion

for /D %%A in (*) do (
	set /a cnt=0
	rem echo Visiting %%A
	for %%F in (%%A\frame_index\*) do (
		rem echo Counting %%F
		set /a cnt+=1		
	)
	echo Frame index for %%A = !cnt!
)