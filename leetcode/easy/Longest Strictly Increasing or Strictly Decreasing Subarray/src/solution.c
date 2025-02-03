#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

int longestMonotonicSubarray( const int* nums, int numsSize ) {
    int maxLen = 1;
    int streak = 1;
    enum {
        RISING,
        FALLING,
        UNKOWN
    } dir = UNKOWN;

    for ( int i = 1; i < numsSize; i++, streak++ ) {
        int diff = nums[i] - nums[i - 1];

        if ( !diff ) {
            if ( streak > maxLen ) {
                maxLen = streak;
            }
            streak = 0;
            dir    = UNKOWN;
            continue;
        }

        if ( dir == UNKOWN ) {
            dir    = diff < 0;
            streak = 1;

            // was rising, know is falling OR was falling, know is rising
        } else if ( (uint32_t) ( diff < 0 ) ^ dir ) {
            if ( streak > maxLen ) {
                maxLen = streak;
            }
            dir = UNKOWN;
            i--;
        }
    }

    return streak > maxLen ? streak : maxLen;
}