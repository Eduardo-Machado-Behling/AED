#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// time (log(min(numA,numB)))
int Gcd( int numA, int numB ) {
    while ( numB != 0 ) {
        int temp = numB;
        numB     = numA % numB;
        numA     = temp;
    }
    return numA;
}

// time (log(min(numsSize, k)) + numsSize)
void rotate( int* nums, int numsSize, int k ) {
    if ( !k || k == numsSize || numsSize == 1 ) {
        return;
    }

    // time (log(min(numsSize, k)))
    size_t rangeFoo = Gcd( numsSize, k );

    // time (gcd(numsSize, k))
    for ( size_t i = 0; i < rangeFoo; i++ ) {
        int temp = nums[i];
        size_t j = i;

        // time (numsSize / gcd(numsSize, k))
        for ( size_t _ = 0; _ < numsSize / rangeFoo; _++ ) {
            j       = ( j + k ) % numsSize;
            int tt  = temp;
            temp    = nums[j];
            nums[j] = tt;
        }
    }
}