@echo    off
cd       ..\..\..\

set      SPARTAN=%cd%\build\binary
set      SRCPATH=%cd%\source
set      BINPATH=%cd%\build\binary
set      DRIVERS=%cd%\build\binary\drivers
set      DEPLOY=%cd%\deploy
set      RAMDISK=%cd%\deploy\ramdisk
set      CDROM=%cd%\deploy\cdrom
set      OUTDIR=%cd%


fasm     %SRCPATH%\kernel\grub\boot.asm %BINPATH%\boot.bin
copy     %BINPATH%\*.exe %RAMDISK%\bin\
copy     %DRIVERS%\*.dll %RAMDISK%\dev\
copy     %BINPATH%\*.dll %RAMDISK%\sys\

mkfs     %RAMDISK%\ %DEPLOY%\ramdisk.img sys/kernel.dll 15 9 3
cd       deploy
mkimg144 -bs %DEPLOY%\fat12.bin -us %DEPLOY%\ramdisk.img -o %OUTDIR%\floppy.img

rebase   %BINPATH%\boot.dll 0x00100080
copy     /b %BINPATH%\boot.bin + %BINPATH%\boot.dll %DEPLOY%\boot.bin
mkiso    %DEPLOY%\blank.iso %DEPLOY%\boot.bin %DEPLOY%\ramdisk.img %OUTDIR%\cdrom.iso
dir
