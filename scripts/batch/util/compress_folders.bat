@echo off
for /D %%X in (*) do "C:\Program Files\7-Zip\7z.exe" a -mx0 "%%X.zip" "%%X\"