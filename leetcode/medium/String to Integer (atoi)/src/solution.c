#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define STR_LEN_CONTRAINT 200
#define NEGATIVE_FLAG UINT8_MAX

struct idStack {
    uint8_t data[STR_LEN_CONTRAINT];
    size_t used;
};

bool LesserIntMem( uint8_t* da, uint8_t* db, uint32_t daSize, uint32_t dbSize ) {
    if ( dbSize < daSize )
        return false;
    else if ( dbSize > daSize )
        return true;

    for ( size_t i = 0; i < daSize; i++ ) {
        if ( db[i] == da[i] )
            continue;
        else
            return db[i] > da[i];
    }

    return false;
}

int myAtoi( char* s ) {
    struct idStack stack = { .data = { 0 },
                             .used = 0 };
    bool started         = false;

    for ( size_t i = 0; s[i]; i++ ) {
        if ( isdigit( s[i] ) ) {
            if ( !started )
                started = true;

            stack.data[stack.used++] = s[i] - '0';
        } else if ( !started ) {
            if ( s[i] == '-' || s[i] == '+' ) {
                if ( s[i] == '-' )
                    stack.data[stack.used++] = NEGATIVE_FLAG;
                started = true;
            } else if ( s[i] != ' ' )
                break;
            continue;
        } else {
            break;
        }
    }

    size_t offset = 0;

    // ignores the negative flag (which always is on *stack.data) and leading zeros
    for ( ; offset < stack.used && !( stack.data[offset] > 0 && stack.data[offset] < 10 ); offset++ )
        ;
    stack.used -= offset;

    uint8_t memMax[] = { 2, 1, 4, 7, 4, 8, 3, 6, 4, 7 };

    if ( LesserIntMem( memMax, stack.data + offset, sizeof( memMax ) / sizeof( int8_t ), stack.used ) ) {
        if ( *stack.data == NEGATIVE_FLAG )
            return INT32_MIN;
        else
            return INT32_MAX;
    }

    int64_t val    = 0;
    uint32_t radix = 1;
    for ( size_t i = 0; i < stack.used; i++, radix *= 10 ) {
        size_t ri = stack.used - 1 - i + offset;

        val += stack.data[ri] * radix;
    }

    if ( *stack.data == NEGATIVE_FLAG )
        val *= -1;
    return val;
}