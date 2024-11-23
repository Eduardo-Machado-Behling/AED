#include "solution.h"

#include <stdlib.h>

int* twoSum( int* nums, int numsSize, int target, int* returnSize ) {
    int* ret = malloc( sizeof( int ) * 2 );

    for ( int i = 0; i < numsSize && ret; i++ ) {
        int search = target - nums[i];
        for ( int j = 0; j < numsSize; j++ ) {
            if ( i == j )
                continue;
            else if ( nums[j] == search ) {
                *returnSize = 2;
                ret[0]      = i;
                ret[1]      = j;
                return ret;
            }
        }
    }

    *returnSize = 0;
    return NULL;
}