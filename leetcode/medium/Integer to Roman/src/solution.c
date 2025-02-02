#include "solution.h"

#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024

// Better solution than mine, didn't thought of doing it like this.
char* intToRoman( int num ) {
    const char* SYMBOLS[]      = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
    const int SYMBOLS_VALUES[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };

    char buffer[BUFFERSIZE] = { 0 };
    int bufferI             = 0;

    for ( int i = 0; num; i += 1 ) {
        for ( ; num >= SYMBOLS_VALUES[i]; num -= SYMBOLS_VALUES[i] ) {
            for ( char* symbolStr = SYMBOLS[i]; *symbolStr; symbolStr++ ) {
                buffer[bufferI++] = *symbolStr;
            }
        }
    }

    char* str = malloc( bufferI + 1 );
    strcpy( str, buffer );
    return str;
}