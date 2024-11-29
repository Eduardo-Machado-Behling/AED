#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define MIN_VALUE_CONTRAINT -10000

int removeDuplicates( int* nums, int numsSize ) {
    int i     = 1;
    int j     = 1;
    int count = 0;

    for ( ; i < numsSize; i++, j++ ) {
        if ( nums[i - 1] >= nums[i] ) {
            if ( nums[i - 1] > nums[i] || ++count > 1 ) {
                while ( ++j < numsSize && ( ( nums[j] < nums[i - 2] ) || ( count > 1 && nums[j] == nums[i - 1] ) ) )
                    ;

                if ( j >= numsSize )
                    return i;

                if ( nums[i - 1] != nums[j] )
                    count = 0;
                count++;

                nums[i]   = nums[j];
                nums[j--] = MIN_VALUE_CONTRAINT - 1;
            }
        } else {
            count = 0;
        }
    }

    return i;
}