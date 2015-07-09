call "C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/vcvarsall.bat" x86

G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff/bin2coff.exe bgs/Mat_W.bin bgs/Mat_W.obj Mat_W

dumpbin.exe /symbols bgs/Mat_W.obj

G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff/bin2coff.exe bgs/SupportVectors.bin bgs/SupportVectors.obj SupportVectors

dumpbin.exe /symbols bgs/SupportVectors.obj

G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff/bin2coff.exe bgs/alpha.bin bgs/alpha.obj alpha

dumpbin.exe /symbols bgs/alpha.obj

G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff/bin2coff.exe bgs/bias.bin bgs/bias.obj bias

dumpbin.exe /symbols bgs/bias.obj

G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff/bin2coff.exe bgs/sigma.bin bgs/sigma.obj sigma

dumpbin.exe /symbols bgs/sigma.obj


lib.exe /out:bgs/bgs.lib bgs/Mat_W.obj bgs/SupportVectors.obj bgs/alpha.obj bgs/bias.obj bgs/sigma.obj
dumpbin.exe /symbols bgs/bgs.lib


pause