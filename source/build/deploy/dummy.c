_declspec(dllexport)
char *dummy(void) {
    return "force relocations";
}

int __stdcall main(int a, int b, int c) {
    return 1;
}