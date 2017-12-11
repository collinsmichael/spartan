#ifndef PIC_H
#define PIC_H

#define PIC1      0x20
#define PIC2      0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define EOI       0x20

void pic_remap(void);

#endif // PIC_H