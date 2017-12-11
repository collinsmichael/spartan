@echo    off
cd       ..\binary\
rebase   kernel.dll 0x00101000
mkiso    blank.iso boot.bin kernel.dll cdrom.iso
