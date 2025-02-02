#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

int reverse( int x ) {
    if ( x == INT32_MIN ) {
        return 0;
    }

    const int threshold = INT32_MAX / 10;

    int num  = 0;
    int sign = x > 0 ? 1 : -1;
    x        = abs( x );

    for ( int i = 0; x; i++ ) {
        if ( num > threshold ) {
            return 0;
        }

        num *= 10;
        num += x % 10;
        x /= 10;
    }

    return num * sign;
}