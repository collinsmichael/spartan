#ifndef STRING_H
#define STRING_H

void *memchr(const void *buffer, int c, int size);
int   memcmp(const void *target, const void *source, int size);
void *memcpy(void *target, const void *source, int size);
void *memset(void *buffer, int c, int size);
 
char *strcat(char *target, const char *source);
char *strchr(const char *text, int c);
int   strcmp(const char *target, const char *source);
char *strcpy(char *target, const char *source);
char *strdup(const char *text);
int   stricmp(const char *target, const char *source);
int   strlen(const char *text);
char *strlwr(char *text);
char *strncat(char *target, const char *source, int size);
int   strncmp(const char *target, const char *source, int size);
char *strncpy(char *target, const char *source, int size);
int   strnicmp(const char *target, const char *source, int size);
int   strnlen(const char *text, int size);
char *strstr(const char *target, const char *source);
char *strupr(char *text);
char  tolower(char c);
char  toupper(char c);
#define strcmpi(s1,s2) stricmp(s1,s2)
#define strncmpi(s1,s2,n) strnicmp(s1,s2,n)

#endif // STRING_H