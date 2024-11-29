#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

// time (log(min(a,b)))
int gcd( int a, int b ) {
    while ( b != 0 ) {
        int temp = b;
        b        = a % b;
        a        = temp;
    }
    return a;
}

// time (log(min(numsSize, k)) + numsSize)
void rotate( int* nums, int numsSize, int k ) {
    if ( !k || k == numsSize || numsSize == 1 )
        return;

    // time (log(min(numsSize, k)))
    size_t range = gcd( numsSize, k );

    // time (gcd(numsSize, k))
    for ( size_t i = 0; i < range; i++ ) {
        int temp = nums[i];
        size_t j = i;

        // time (numsSize / gcd(numsSize, k))
        for ( size_t _ = 0; _ < numsSize / range; _++ ) {
            j       = ( j + k ) % numsSize;
            int tt  = temp;
            temp    = nums[j];
            nums[j] = tt;
        }
    }
}