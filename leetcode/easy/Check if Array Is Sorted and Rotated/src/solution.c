#include "solution.h"

bool check( const int* nums, int numsSize ) {
    bool seqBreak = false;
    for ( size_t i = 1; i < numsSize; i++ ) {
        if ( nums[i] < nums[i - 1] ) {
            if ( seqBreak ) {
                return false;
            }
            seqBreak = true;
        }
        if ( seqBreak ) {
            if ( nums[0] < nums[i] ) {
                return false;
            }
        }
    }

    return true;
}