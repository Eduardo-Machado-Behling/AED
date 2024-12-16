#include "solution.h"

int hammingWeight( int n ) {
    uint32_t count = 0;
    for ( ; n; n >>= 1 ) {
        count += n & 0x1;
    }

    return count;
}