int shell(void);

_declspec(dllexport)
void print(char *text) {
    char *stdout = (char*)0xB8000;
    while (*text) {
        *stdout++ = *text++;
        stdout++;
    }
}

_declspec(dllexport)
void kernel(void) {
    for (;;) shell();
}

void main(void) {
    kernel();
}