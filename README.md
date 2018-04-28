# Spartan OS
Spartan OS is an Operating System developed as a Final Year Project for
the Bachelor of Science (Hons) in Software Development at Limerick Institute of Technology
    http://www.lit.ie/Courses/LC233/default.aspx

Spartan OS is a Multi-Tasking Operating System which explores Human-Machine Interaction.
Explores Device IO and Human IO, through the typical point and click GUI interface,
in order to allow humans to speak to machines on human terms.
Delivered within a 6-month timeframe, starting from a truly blank canvas. Achieves
optimal Inter-Process Communication (Data Flow Management) which is invariant to the
volume of data, using shared memory and an advanced zero-copy operation policy, to
direct the flow of data throughout the system without copying a single byte. Supports
the keyboard, mouse and video display adapter, despite having no access to BIOS (Firmware)
services, by utilizing industry standard protocols to communicate directly with devices
on the same terms as would the BIOS itself. Achieves ergonomic Human IO, by implementing
mathematical findings such as Fitts Law, to increase human comfort and productivity in a
controlled and configurable manner. The system is capable of error detection and recovery,
through the use of checksums and restore points, which hardens the system and provides
a high level of reliability with a capacity to self-heal and recover from otherwise
fatal error conditions.

Spartan OS is a Multi-Tasking 32-bit Protected Mode Operating System featuring
a Graphical User Interface with file associations and is designed with usability in mind.
Spartan OS uses an in-memory file system (Ram Disk), and has drivers for the PC Speaker,
the Serial Port and the PS/2 keyboard and mouse.

This repository contains both the software implementation, and the research and design documentation.
The iconography included within the RamDisk directory are derivative works with the originals
sourced from https://icons8.com/ and the imagery contained within the RamDisk/Media/Images
and RamDisk/Media/Video directories were sourced from Wikipedia

# Size of Code Base
The following statistics do not include the application source code
since the applications are for demonstration purposes only.

| LANG | KERNEL | SHELL | DESKTOP | TOTAL |
| :--- | ---: |  ---: | ---: | ---: |
| C    |   3740 |  5860 |     728 | 10328 |
| H    |   1906 |   651 |      66 |  2623 |
| ASM  |    701 |     0 |       0 |   701 |
| ANY  |   6347 |  6511 |     794 | 13652 |

# Screenshots  
Animated GIF demonstrating file associations
![Alt text](/ScreenShots/Spartan.gif?raw=true "Spartan")

Image demonstrating multi-tasking
![Alt text](/ScreenShots/12-image-viewer.png?raw=true "Image Viewer")
