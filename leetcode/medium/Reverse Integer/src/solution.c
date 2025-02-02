#include "solution.h"

#include <limits.h>
#include <stdint.h>

int reverse( int x ) {
    int64_t num = 0;

    for ( int i = 0; x; i++ ) {
        num *= 10;
        num += x % 10;
        x /= 10;
    }

    return num > INT32_MAX || num < INT32_MIN ? 0 : num;
}