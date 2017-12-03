@echo    off
cd       ..\..\..\

set      SPARTAN=%cd%\build\binary
set      BINPATH=%cd%\build\binary
set      DRIVERS=%cd%\build\binary\drivers
set      DEPLOY=%cd%\deploy
set      RAMDISK=%cd%\deploy\ramdisk
set      CDROM=%cd%\deploy\cdrom
set      OUTDIR=%cd%

copy     %BINPATH%\*.exe %RAMDISK%\bin\
copy     %DRIVERS%\*.dll %RAMDISK%\dev\
copy     %BINPATH%\*.dll %RAMDISK%\sys\
rebase   %BINPATH%\boot.dll 0x00100800
copy     /b %BINPATH%\boot.bin + %BINPATH%\boot.dll %CDROM%\boot\boot.bin

mkfs     %RAMDISK%\ %DEPLOY%\ramdisk.img sys/kernel.dll 15 9 3
cd       deploy
mkimg144 -bs %DEPLOY%\fat12.bin -us %DEPLOY%\ramdisk.img -o %OUTDIR%\floppy.img
copy     /b %DEPLOY%\eltorito.bin + %DEPLOY%\floppy.img     %OUTDIR%\cdrom.iso
dir
