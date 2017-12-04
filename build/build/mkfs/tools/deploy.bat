fasm ..\source\stub\stub.asm ..\library\stub.exe
fasm ..\source\boot\boot.asm ..\library\boot.exe
copy ..\disk\sys\*.lib ..\library
del  ..\disk\sys\*.exp
del  ..\disk\sys\*.ilk
del  ..\disk\sys\*.lib
del  ..\disk\sys\*.pdb
mkfs ..\disk ..\binary\ramdisk.img sys/kernel.dll
copy /b "..\tools\boot.exe" + "..\binary\ramdisk.img" "..\tools\ramdisk.img"
mkimg144.exe -bs "..\tools\fat12.bin" -o "..\binary\floppy.img" -us "..\tools\ramdisk.img"
copy /b "..\tools\eltorito.bin" + "..\binary\floppy.img" "..\binary\cdrom.iso"
