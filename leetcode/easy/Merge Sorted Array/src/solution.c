#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

// Another solution based on hints from the community
void merge( int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n ) {
    int* n1 = nums1 + m - 1;
    int* n2 = nums2 + n - 1;

    for ( size_t ri = nums1Size; ri > 0 && n2 >= nums2; ri-- ) {
        if ( n1 >= nums1 && *n1 > *n2 ) {
            nums1[ri - 1] = *n1--;
        } else {
            nums1[ri - 1] = *n2--;
        }
    }
}