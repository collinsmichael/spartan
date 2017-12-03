/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : shell/shell/shell.c                                            */
/* file date : 2017/12/03                                                     */
/* file info : bourne shell commandline interpreter                           */
/* ************************************************************************** */
#include <shell.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

int main(int argc, char *argv[]) {
    for (;;);
    return 1;
}