#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

import IMemory  *Memory;
import IThread  *Thread;
import IProcess *Process;
import IPool    *Pool;
import IPipe    *Pipe;

export bool Install_DeathLoop(void *callback);
void DeathLoop(char *error);

int CrtArgC(char *text) {
	int argc = 0;
	while (*text >= ' ') {
		char c;
        // skip white space
		while (*text == ' ') text++;
		c = *text;

		// grab quoted arguments
		if (c == '"') {
			text++;
			argc++;
			while (*text != c) {
                if (*text++ < ' ') break;
            }

		// grab non quoted arguments
		} else if (*text > ' ') {
			argc++;
			while (*text++ > ' ');
		}
	}
	return argc;
}

char **CrtArgV(int argc, char *text) {
	int arg = 0;

	char **argv = (char**)Memory->Alloc((argc + 1)*sizeof(char**));
	if (!argv) return 0;

	while (*text >= ' ') {
		char c;

        // skip white space
		while (*text == ' ') text++;

        // grab quoted arguments
		c = *text;
		if (c == '"') {
			argv[arg++] = ++text;
			while (*text != c) {
				if (*text < ' ') break;
				text++;
			}
			*text++ = 0;

        // grab non quoted arguments
		} else if (*text > ' ') {
			argv[arg++] = text;
			while (*text > ' ') text++;
			*text++ = 0;
		}
	}
	argv[argc] = "";
	return argv;
}

int CrtStart(void) {
    Install_DeathLoop(DeathLoop);

    CPcb *pcb = Process->GetPcb();
    CPeb *peb = Process->GetPeb();
    if (!pcb || !peb) return -1;

    int evtsize  = align(sizeof(CEvent), 64);
    int evtcount = 1*MB/evtsize;
    int winsize  = align(sizeof(CWindow), 64);
    int wincount = 4*MB/winsize;
    CPool *evtpool = peb->EventPool;
    CPool *winpool = peb->WindowPool;
    Pool->CreateMeta(evtpool, evtcount, evtsize, evtpool->Node, (char*)pcb->Events, "EVTS");
    Pool->CreateMeta(winpool, wincount, winsize, winpool->Node, (char*)pcb->Windows, "WINS");

    int (*main)(int,char**) = (int(*)(int,char**))pcb->AppMain;
    if (!main) return -1;

    peb->ArgC = CrtArgC(peb->CommandLine);
    peb->ArgV = CrtArgV(peb->ArgC, peb->CommandLine);

    Debug(" Crt0 cmd= (%X) %s\n", peb->CommandLine, peb->CommandLine);
    Debug(" Crt0 ArgC=%d ArgV=%X\n", peb->ArgC, peb->ArgV);
    return main(peb->ArgC, peb->ArgV);
}

// true entry point for all applications
int _stdcall start(int a, int b, int c) {
    Thread->Yield();
    CrtStart();
    Process->Exit(0);
    return 0;
}