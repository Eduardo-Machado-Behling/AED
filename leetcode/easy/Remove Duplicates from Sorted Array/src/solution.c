#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int removeDuplicates( int* nums, int numsSize ) {
    size_t s = 1;
    size_t i = 1;

    for ( ; i < numsSize; i++ ) {
        if ( nums[i - 1] >= nums[i] ) {
            while ( ++s < numsSize && nums[s] <= nums[i - 1] )
                ;

            if ( s >= numsSize )
                break;
            nums[i] = nums[s];
        }
    }

    return i;
}