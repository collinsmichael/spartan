#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

import IMonitor *Monitor;

extern void *memory[];
extern void *monitor[];
extern void *thread[];

void **modules[] = {
    memory,
    thread,
    0
};

char text[4096];

export bool TestRunner(int stress) {
    Logger("\n");
    Logger(" *****************************************************\n");
    Logger(" Test Runner is verifying system quality and integrity\n");
    Logger(" *****************************************************\n");
    Monitor->ProtectAll();
    bool status = true;
    for (int i = 0; status; i++) {
        Logger("\n");
        void **mod = modules[i];
        if (!mod) break;

        bool (*SetUp)(void) = mod[0];
        if (SetUp) SetUp();

        for (int j = 0; status; j++) {
            int level = (int)mod[j*2 + 2];
            if (level > stress) continue;
            bool (*unit)(int) = mod[j*2 + 3];
            if (level == 0 || !unit) break;

            status = unit(stress);
            if (!status) break;
        }

        bool (*TearDown)(void) = mod[1];
        if (!TearDown) TearDown();
    }
    Monitor->RecoverAll();
    Logger(" *****************************************************\n");
    Logger(" Tests Finished with %serrors\n", (status) ? "no " : "");
    Logger(" *****************************************************\n");
    Logger("\n");
    return status;
}