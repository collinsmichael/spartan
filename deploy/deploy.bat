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
copy     %BINPATH%\*.exe %RAMDISK%\bin\
copy     %DRIVERS%\*.dll %RAMDISK%\dev\
copy     %BINPATH%\*.dll %RAMDISK%\sys\

copy     %BINPATH%\*.dll         %DEPLOY%\
copy     %BINPATH%\drivers\*.dll %DEPLOY%\
copy     %BINPATH%\*.pdb         %DEPLOY%\
copy     %BINPATH%\drivers\*.pdb %DEPLOY%\

rem mkrd     0x00101000 %RAMDISK%\ %WORKAREA%\ramdisk.img sys/kernel.dll 15 9 3
cd       deploy
mkimg144 -bs %WORKAREA%\fat12.bin -us %WORKAREA%\ramdisk.img -o %OUTDIR%\floppy.img

rebase   %BINPATH%\boot.dll 0x00100080
copy     /b %BINPATH%\boot.bin + %BINPATH%\boot.dll %WORKAREA%\boot.bin
mkiso    %WORKAREA%\blank.iso %WORKAREA%\boot.bin %WORKAREA%\ramdisk.img %OUTDIR%\cdrom.iso
dir
