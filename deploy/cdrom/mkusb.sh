#!/bin/bash

# build the kernel and cdrom.iso
make all
cp obj/kernel.bin iso/boot/kernel.bin
grub-mkrescue -o cdrom.iso iso/

#flash grub and cdrom.iso to a usb with no partitions
#umount /dev/sdb
sudo mkfs.vfat -F 32 -n OSDEV -I /dev/sdb
#sudo grub-install --target=i386-pc --root-directory=./iso --no-floppy --recheck --force /dev/sdb
sudo grub-install --target=i386-pc --root-directory=/media/mike/OSDEV --no-floppy --recheck --force /dev/sdb
