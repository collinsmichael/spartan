#ifndef KERNEL_H
#define KERNEL_H
#include <oop.h>
#include <Library.h>

#include <Kernel\BootLoad\Intel\Intel.h>
#include <Kernel\BootLoad\Intel\Fpu.h>
#include <Kernel\BootLoad\Intel\Pic.h>
#include <Kernel\BootLoad\Intel\Tss.h>
#include <Kernel\BootLoad\MultiBoot\MultiBoot.h>
#include <Kernel\BootLoad\PcBios\Cga.h>
#include <Kernel\BootLoad\PcBios\Com.h>
#include <Kernel\BootLoad\PcBios\Pit.h>
#include <Kernel\BootLoad\PcBios\Ps2.h>
#include <Kernel\BootLoad\PcBios\Rtc.h>
#include <Kernel\BootLoad\PcBios\PcBios.h>
#include <Kernel\BootLoad\BootLoad.h>

#include <Kernel\Device\Drivers\RamDisk.h>
#include <Kernel\Device\Drivers\Vesa.h>
#include <Kernel\Device\FileiO\FileIO.h>
#include <Kernel\Device\FileiO\Pipe.h>
#include <Kernel\Device\Device.h>

#include <Kernel\Memory\Heap\Memory.h>
#include <Kernel\Memory\Heap\Pool.h>
#include <Kernel\Memory\Paging\Paging.h>

#include <Kernel\Monitor\Logger.h>
#include <Kernel\Monitor\MemMap.h>
#include <Kernel\Monitor\Monitor.h>
#include <Kernel\Monitor\SysCall.h>

#include <Kernel\Thread\Exception\Exception.h>
#include <Kernel\Thread\Loader\Exe.h>
#include <Kernel\Thread\Loader\ExeLoader.h>
#include <Kernel\Thread\Process\Process.h>
#include <Kernel\Thread\Process\Thread.h>

#endif KERNEL_H