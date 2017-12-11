@echo    off
rebase   kernel.exe 0x00100800
mkiso    blank.iso boot.bin kernel.exe cdrom.iso
dir
