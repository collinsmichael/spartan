#!/bin/bash

# build the kernel and cdrom.iso
make all
cp obj/kernel.bin iso/boot/kernel.bin
grub-mkrescue -o cdrom.iso iso/
qemu-system-x86_64 -kernel iso/boot/kernel.bin
