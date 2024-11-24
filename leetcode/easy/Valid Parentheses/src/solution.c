#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define MAX_AMOUNT_OF_SYMBOLS_IN_STACK 5000

bool isValid( char* s ) {
    struct {
        char buf[MAX_AMOUNT_OF_SYMBOLS_IN_STACK];
        size_t used;
    } stack = { .used = 0, .buf = { 0 } };

    for ( size_t i = 0; s[i]; i++ ) {
        switch ( s[i] ) {
            case '(':
            case '[':
            case '{':
                // can't close everyone
                if ( stack.used == MAX_AMOUNT_OF_SYMBOLS_IN_STACK )
                    return false;
                stack.buf[stack.used++] = s[i];
                break;

            case ')':
            case ']':
            case '}':
                if ( stack.used == 0 ) {
                    return false;
                } else if ( s[i] == ')' ) {
                    if ( stack.buf[--stack.used] != '(' )
                        return false;
                } else if ( s[i] != stack.buf[--stack.used] + 2 ) {
                    return false;
                }
                break;

            default:
                break;
        }
    }

    return stack.used == 0;
}