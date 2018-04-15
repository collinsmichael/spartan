#include <Library.h>
#include <Kernel.h>

extern IDevice *Device;

u32 RoundRobin(u32 err, u32 esp);

u32 hz;
u32 ticks;
u32 seconds;
u32 timestamp;

void IClock_CmosDate(u8 *datetime) {
    static u8 cmos[] = "\x00\x02\x04\x07\x08\x09\x32\x0A\x0B";
    u8 older[16];
    u8 newer[16];
    u8 cent = 0;

    // wait for old and new to match in case an update fires
    stosd(newer, 0, sizeof(newer)/4);
    do {
        movsd(older, newer, sizeof(newer)/4);
        for (int i = 0; i < 8; i++) {
            outb(CMOS_INDEX, CMOS_NO_NMI | cmos[i]);
            newer[i] = inb(CMOS_DATA);
        }
        outb(CMOS_INDEX, CMOS_NO_NMI | 108);
        cent = inb(CMOS_DATA);
        if (cent == 0) continue;
        outb(CMOS_INDEX, CMOS_NO_NMI | cent);
        newer[CMOS_CENTURY] = inb(CMOS_DATA);
    } while (cmpsb(older, newer, sizeof(newer)/4));

    // binary coded decimal
    if (!(newer[CMOS_STATUSB] & 0x04)) {
        for (int i = 0; i < 7; i++) {
            newer[i] = (newer[i] % 16) + (newer[i]/16)*10;
        }
    }

    // 12 hour / 24 hour
    if (!(newer[CMOS_STATUSB] & 0x02) && (newer[CMOS_HOUR] & 0x80)) {
        newer[CMOS_HOUR] = ((newer[CMOS_HOUR] & 0x7F) + 12) % 24;
    }

    // centuary wrap around 1990-1999 ... 2000-2089
    if (cent == 0) {
        if (newer[CMOS_YEAR] > 90) {
            newer[CMOS_CENTURY] = 19;
        } else {
            newer[CMOS_CENTURY] = 20;
        }
    }
    movsb(datetime, newer, 7);
}

static void IClock_Flush(void) {
    outb(CMOS_INDEX, CMOS_RTC_C);
    inb(CMOS_DATA);
}

static bool IClock_Now(u8 *now) {
    u8 cmos[8];
    IClock_CmosDate(cmos);

    now[0] = (hz) ? (ticks*256/hz) : 0;
    now[1] = cmos[CMOS_SECOND];
    now[2] = cmos[CMOS_MINUTE];
    now[3] = cmos[CMOS_HOUR];
    now[4] = cmos[CMOS_DAY];
    now[5] = cmos[CMOS_MONTH];
    now[6] = cmos[CMOS_YEAR];
    now[7] = cmos[CMOS_CENTURY];
    return true;
}

static u32 IClock_GetTicks(void) {
    return seconds*hz + ticks;
}

static u32 IClock_GetFreq(void) {
    return hz;
}

static u32 ClockIsr(u32 err, u32 esp) {
    esp = RoundRobin(err, esp);
  //if (ticks == 0) Logger(" [TIME] %d sec\n", seconds);
    ticks = ++ticks & (hz-1);
    if (ticks == 0) seconds++;
    outb(CMOS_INDEX, CMOS_RTC_C); // select RTC C
    inb(CMOS_DATA); // flush RTC C
    return esp;
}

bool InstallClock(u8 *base, u32 size) {
    seconds = 0;
    ticks = 0;
    hz = 1024;
    return true;
}

bool EnableClock(u8 *base, u32 size) {
    outb(CMOS_INDEX, CMOS_MASK+CMOS_RTC_B); // select RTC B
    u8 b = inb(CMOS_DATA) & 0x0F;           // get register value
    outb(CMOS_DATA, b|CMOS_INT);            // enable interrupts
    outb(CMOS_INDEX, CMOS_MASK+CMOS_RTC_A); // set index to register A, disable NMI
    char a = inb(CMOS_DATA);                // get initial value of register A
    outb(CMOS_INDEX, CMOS_MASK+CMOS_RTC_A); // reset index to register A, disable NMI
    outb(CMOS_DATA, (a & 0xF0)|RTC_1024HZ); // write only our rate to A. Note, rate is the bottom 4 bits.
    outb(CMOS_INDEX, CMOS_RTC_C);           // select RTC C
    inb(CMOS_DATA);                         // flush RTC C
    Device->Latch(IRQ_RTC, ClockIsr);
    return true;
}

IClock clock = {
    IClock_Flush,
    IClock_CmosDate,
    IClock_Now,
    IClock_GetTicks,
    IClock_GetFreq
};

export IClock *Clock = &clock;