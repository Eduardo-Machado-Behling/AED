#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define MAX_ARRAY_SIZE 200

void merge( int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n ) {
    int aux[MAX_ARRAY_SIZE] = { 0 };

    size_t i1 = 0;
    size_t i2 = 0;
    for ( size_t i = 0; i < nums1Size; i++ ) {
        if ( i1 < m && i2 < n ) {
            if ( nums1[i1] < nums2[i2] ) {
                aux[i] = nums1[i1++];
            } else {
                aux[i] = nums2[i2++];
            }
        } else if ( i1 < m ) {
            aux[i] = nums1[i1++];
        } else {
            aux[i] = nums2[i2++];
        }
    }

    memcpy( nums1, aux, nums1Size * sizeof( int ) );
}