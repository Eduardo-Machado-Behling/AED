#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int majorityElement( int* nums, int numsSize ) {
    int counter = 1;
    int value   = nums[0];
    for ( size_t i = 1; i < numsSize; i++ ) {
        if ( !counter ) {
            value   = nums[i];
            counter = 1;
        } else if ( value != nums[i] ) {
            counter--;
        } else {
            counter++;
        }
    }

    return value;
}