#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int removeElement( int* nums, int numsSize, int val ) {
    int* forwards  = nums;
    int* backwards = nums + numsSize;

    for ( ; forwards < backwards; forwards++ ) {
        if ( *forwards == val ) {
            while ( --backwards > forwards && *backwards == val )
                ;
            if ( backwards <= forwards ) {
                break;
            }
            *forwards = *backwards;
        }
    }

    return forwards - nums;
}