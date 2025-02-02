#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include <math.h>

bool isPalindrome( int x ) {
    if ( x < 0 ) {
        return false;
    }

    if ( x == 0 ) {
        return true;
    }

    long digitsAmount = log10l( x );
    long mask         = powl( 10, digitsAmount );
    for ( size_t i = 0, j = digitsAmount; j > i; i++, j-- ) {
        if ( ( x / mask ) % 10 != x % 10 ) {
            return false;
        }

        x /= 10;
        mask /= 100;
    }

    return true;
}