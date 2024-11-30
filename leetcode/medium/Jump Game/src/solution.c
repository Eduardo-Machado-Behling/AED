#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUMS_MAX_SIZE 10000

enum state_t {
    State_UNKOWN,
    State_TRUE,
    State_FALSE
};

bool Traverse_r( uint8_t* cache, const int* nums, const int index, int numsSize ) {
    if ( cache[index] != State_UNKOWN ) {
        return cache[index] == State_TRUE;
    }

    int jumps = nums[index];
    if ( index == numsSize - 1 ) {
        cache[index] = State_TRUE;
        return true;
    }

    for ( int i = 1; index + i < numsSize && i <= jumps; i++ ) {
        if ( Traverse_r( cache, nums, index + i, numsSize ) ) {
            return true;
        }
    }

    cache[index] = State_FALSE;
    return false;
}

bool canJump( const int* nums, int numsSize ) {
    // Yes, I'm caching, I don't know the magical O(n) solution YET.
    uint8_t cache[NUMS_MAX_SIZE] = { 0 };
    return Traverse_r( cache, nums, 0, numsSize );
}