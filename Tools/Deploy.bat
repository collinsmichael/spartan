@echo  off

cd     ..\
set    BIN=%CD%\Binary\
set    LIB=%CD%\Library\
set    OBJ=%CD%\Object\
set    SRC=%CD%\Source\
set    TOOL=%CD%\Tools\
set    DISK=%CD%\RamDisk\

move  %BIN%Blocker.exe %DISK%Apps\Blocker\
move  %BIN%Gui.exe      %DISK%Apps\Gui\
move  %BIN%Calc.exe     %DISK%Apps\Calc\
move  %BIN%Chat.exe     %DISK%Apps\Chat\
move  %BIN%Fps.exe      %DISK%Apps\Fps\
move  %BIN%Txt.exe      %DISK%Apps\Txt\
move  %BIN%Settings.exe %DISK%Apps\Settings
move  %BIN%Cpu.exe      %DISK%Apps\Settings
move  %BIN%KeyBoard.exe %DISK%Apps\Settings
move  %BIN%Mouse.exe    %DISK%Apps\Settings
move  %BIN%Monitor.exe  %DISK%Apps\Settings
move  %BIN%Speaker.exe  %DISK%Apps\Settings
move  %BIN%Network.exe  %DISK%Apps\Settings
move  %BIN%RamDisk.exe  %DISK%Apps\Settings

copy   %BIN%*.dll %DISK%System\
copy   %BIN%*.exe %DISK%Apps\
copy   %BIN%*.lib %OBJ%

del    %BIN%*.exp
del    %BIN%*.exe
del    %BIN%*.dll
del    %BIN%*.lib

fasm  %SRC%Kernel\BootLoad\Intel\Fpu.asm %OBJ%Fpu.obj
fasm  %SRC%Kernel\BootLoad\Intel\Intel.asm %OBJ%Intel.obj
fasm  %SRC%Kernel\BootLoad\MultiBoot\boot.asm %BIN%boot.bin
mkrd  0x00101000 %DISK% %BIN%disk.img System/Kernel.dll
mkiso %TOOL%blank.iso %BIN%boot.bin %BIN%disk.img %BIN%cdrom.iso

echo  Missing fasm?  https://flatassembler.net/download.php
echo  Missing bochs? http://bochs.sourceforge.net/
