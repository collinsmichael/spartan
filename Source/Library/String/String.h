#ifndef LIBRARY_STRING_H
#define LIBRARY_STRING_H

char ToLower(char c);
char ToUpper(char c);
void *MemChr(const char *buffer, int c, int size);
int   MemCmp(void *dst, void *src, int len);
void *MemCpy(void *dst, void *src, int len);
void *MemSet(void *dst, int val, int len);
char *StrCat(char *target, const char *source);
int   StrLen(const char *text);
char *StrChr(const char *text, int c);
int   StrCmp(const char *target, const char *source);
char *StrCpy(char *target, const char *source);
int   StrnCmp(const char *target, const char *source, int size);
char *StrnCpy(char *target, const char *source, int size);
int   StrnLen(const char *text, int size);
char *StrDup(const char *text);
int   StriCmp(const char *target, const char *source);
int   StrniCmp(const char *target, const char *source, int size);
char *StrLower(char *text);
char *StrUpper(char *text);
char *StrnCat(char *target, const char *source, int size);
char *StrStr(const char *target, const char *source);

#endif LIBRARY_STRING_H