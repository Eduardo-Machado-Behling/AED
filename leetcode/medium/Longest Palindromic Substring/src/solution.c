#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

char* longestPalindrome( char* s ) {
    struct {
        size_t len;
        char* start;
    } info = { .len = 1, .start = s };

    size_t len = 0;
    for ( ; s[len]; len++ ) {

        for ( size_t i = 0; i < 2; i++ ) {
            char* forwards = s + len + 1;
            if ( i == 1 ) {
                if ( forwards[-1] == *forwards ) {
                    ++forwards;
                } else {
                    break;
                }
            }
            char* backwards = s + len - 1;

            size_t pal_len = 1 + i;
            for ( ; backwards >= s && forwards[-1] != '\0'; backwards--, forwards++ ) {
                if ( *backwards != *forwards ) {
                    break;
                } else {
                    pal_len += 2;
                }
            }
            if ( pal_len > info.len ) {
                info.len   = pal_len;
                info.start = backwards + 1;
            }
        }
    }

    char* str = malloc( ( info.len + 1 ) * sizeof( char ) );
    memcpy( str, info.start, info.len );
    str[info.len] = '\0';
    return str;
}