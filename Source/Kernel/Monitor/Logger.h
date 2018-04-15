#ifndef KERNEL_MONITOR_LOGGER_H
#define KERNEL_MONITOR_LOGGER_H
#include <oop.h>

#ifndef OPTIMIZED
int Debug(char *format, ... );
int Logger(char *format, ... );
#define Info(format, ...) Logger(" [info] " format, __VA_ARGS__)
#define Pass(format, ...) Logger(" [pass] " format, __VA_ARGS__)
#define Fail(format, ...) Logger(" [FAIL] " format, __VA_ARGS__)
#else
int Debug(char *format, ... );
#define Logger(a, ...) do {} while (0)
#define Info(a, ...) do {} while (0)
#define Pass(a, ...) do {} while (0)
#define Fail(a, ...) do {} while (0)
#endif



#endif KERNEL_MONITOR_LOGGER_H