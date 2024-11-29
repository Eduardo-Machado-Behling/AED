#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int compare( const void* a, const void* b ) {
    return *(int*) a - *(int*) b;
}

int majorityElement( int* nums, int numsSize ) {
    qsort( nums, numsSize, sizeof( int ), compare );

    int target = numsSize / 2;
    int count  = 1;
    for ( size_t i = 1; i <= numsSize; i++, count++ ) {
        if ( numsSize == i || nums[i - 1] != nums[i] ) {
            if ( count > target ) {
                return nums[i - 1];
            } else {
                count = 1;
            }
        }
    }

    return nums[0];
}