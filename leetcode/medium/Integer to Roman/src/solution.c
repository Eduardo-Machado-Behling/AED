#include "solution.h"

#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024

char* intToRoman( int num ) {
    const char SYMBOLS[] = { 'I', 'V', 'X', 'L', 'C', 'D', 'M' };

    char buffer[BUFFERSIZE] = { 0 };
    int bufferIndex         = sizeof( buffer ) - 2;

    for ( int i = 2; num; i += 2 ) {
        int digit = num % 10;
        if ( digit <= 3 ) {
            for ( size_t j = 0; j < digit; j++ ) {
                buffer[bufferIndex--] = SYMBOLS[i - 2];
            }
        } else {
            if ( digit <= 5 ) {
                buffer[bufferIndex--] = SYMBOLS[i - 1];
                for ( size_t j = 0; j < 5 - digit; j++ ) {
                    buffer[bufferIndex--] = SYMBOLS[i - 2];
                }
            } else if ( digit <= 8 ) {
                for ( size_t j = 0; j < digit - 5; j++ ) {
                    buffer[bufferIndex--] = SYMBOLS[i - 2];
                }
                buffer[bufferIndex--] = SYMBOLS[i - 1];
            } else {
                buffer[bufferIndex--] = SYMBOLS[i];
                for ( size_t j = 0; j < digit - 8; j++ ) {
                    buffer[bufferIndex--] = SYMBOLS[i - 2];
                }
            }
        }

        num /= 10;
    }

    char* str = malloc( sizeof( buffer ) - bufferIndex );
    strcpy( str, buffer + bufferIndex + 1 );
    return str;
}