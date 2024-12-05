#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool canMakeSubsequence( const char* str1, const char* str2 ) {
    size_t j = 0;
    for ( size_t i = 0; str1[i] && str2[j]; i++ ) {
        int distance = str2[j] - str1[i];

        if ( distance == -25 || distance == 1 || distance == 0 ) {
            j++;
        }
    }

    return str2[j] == '\0';
}