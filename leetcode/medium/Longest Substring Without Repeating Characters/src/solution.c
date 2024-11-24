#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define SYMBOLS_AMOUNT 97
#define SYMBOLS_RANGE_START ' '

int lengthOfLongestSubstring( char* s ) {
    char lookup[SYMBOLS_AMOUNT] = { 0 };

    int max   = 0;
    int count = 0;

    for ( size_t i = 0; true; i++ ) {
        if ( s[i] == '\0' ) {
            if ( count > max )
                max = count;
            break;
        }

        int mapping = s[i] - SYMBOLS_RANGE_START;
        if ( lookup[mapping]++ ) {
            if ( count > max )
                max = count;
            memset( lookup, 0, sizeof( lookup ) );
            i -= count;
            count = 0;
        } else {
            count++;
        }
    }

    return max;
}