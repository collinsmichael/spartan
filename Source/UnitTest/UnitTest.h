#ifndef UNITTEST_H
#define UNITTEST_H
#include <oop.h>


#define EXPECT(expr, format, ...) do {\
    if (!(expr)) {\
        extern char text[4*KB];\
        Logger("\n");\
        Logger(" [FAIL] %s\n", __FUNCTION__);\
        Logger("        file = %s\n", __FILE__);\
        Logger("        line = %d\n", __LINE__);\
        Logger(format "\n\n", __VA_ARGS__);\
        return false;\
    }\
} while (0)

#define PASS do {\
    Logger(" [pass] %s\n", __FUNCTION__);\
    return true;\
} while (0)

bool TestRunner(int stress);

#endif UNITTEST_H