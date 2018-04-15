#include <Kernel.h>

void SetUpClockPaging(CPeb *peb) {
    // set up clock replacement
    peb->ClockFront = 0;
    peb->ClockBack  = 0;
}

static void DiskSwap(int *source, int *target) {
    for (int i = 0; i < 4*KB/4; i++) {
        int t = target[i];
        target[i] = source[i];
        source[i] = t;
    }
}


void ClockPaging(CPeb *peb, int *logical, int *sector) {
    for (;;) {
        // front hand marks pages as unused (least recently used)
        for (;;) {
            // skip unmapped pages
            int clock = peb->ClockFront++;
            int dir = ((clock >> 22) % KB)/sizeof(void*);
            if (peb->Pages[dir] == 0) continue;

            int *table = (int*)peb->Pages[dir];
            int tbl = ((clock >> 12) % KB)/sizeof(void*);
            if (table[tbl] == 0) continue;

            // toggle accessed bit and stop on first unused page
            int accessed = PG_ACCESSED & table[tbl];
            table[tbl] &= (~PG_ACCESSED);
            if (accessed) continue;
        }

        // rear hand replaces unused pages (second chance)
        for (;;) {
            // skip unmapped pages
            int clock = peb->ClockBack++;
            int dir = ((clock >> 22) % KB)/sizeof(void*);
            if (peb->Pages[dir] == 0) continue;

            int *table = (int*)peb->Pages[dir];
            int tbl = ((clock >> 12) % KB)/sizeof(void*);
            if (table[tbl] == 0) continue;

            // locate and swap out the first unused page
            int accessed = PG_ACCESSED & (int)table[tbl];
            if (!accessed) {
                DiskSwap(logical, (int*)table[tbl]);
                table[tbl] = (int)sector;
                return;
            }
        }
    }
}