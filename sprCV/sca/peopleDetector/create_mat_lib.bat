call "C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/vcvarsall.bat" x86_amd64

G:/data/sebastianvilla139/work/survmantics/github/sca/peopleDetector/bin2coff/bin2coff.exe mat_lib/image.bin mat_lib/image.obj image 64bit
dumpbin.exe /symbols mat_lib/image.obj

G:/data/sebastianvilla139/work/survmantics/github/sca/peopleDetector/bin2coff/bin2coff.exe mat_lib/image_rc.bin mat_lib/image_rc.obj image_rc 64bit
dumpbin.exe /symbols mat_lib/image_rc.obj

lib.exe /out:mat_lib/mat_lib.lib mat_lib/image.obj mat_lib/image_rc.obj
dumpbin.exe /symbols mat_lib/mat_lib.lib

pause