void print(char *text);

_declspec(dllexport)
void shell(void) {
    print("Hello World");
    for (;;);
}

void main(void) {
    shell();
}