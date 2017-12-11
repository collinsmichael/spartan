@echo    off
cd       ..\..\..\

set      SPARTAN=%cd%\build\binary
set      SRCPATH=%cd%\source
set      BINPATH=%cd%\build\binary
set      DRIVERS=%cd%\build\binary\drivers
set      DEPLOY=%cd%\deploy
set      WORKAREA=%cd%\deploy\workarea
set      RAMDISK=%cd%\deploy\ramdisk
set      CDROM=%cd%\deploy\cdrom
set      OUTDIR=%cd%

fasm     %SRCPATH%\kernel\grub\boot.asm %BINPATH%\boot.bin
copy     %BINPATH%\*.dll         %DEPLOY%\
copy     %BINPATH%\drivers\*.dll %DEPLOY%\
copy     %BINPATH%\*.pdb         %DEPLOY%\
copy     %BINPATH%\drivers\*.pdb %DEPLOY%\

cd       %DEPLOY%\
dir
