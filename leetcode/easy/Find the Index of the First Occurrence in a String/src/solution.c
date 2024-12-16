#include "solution.h"

int strStr( const char* haystack, const char* needle ) {
    int j = 0;
    int i = 0;

    for ( ; haystack[i] && needle[j]; i++ ) {
        if ( haystack[i] == needle[j] ) {
            ++j;
        } else if ( j ) {
            i -= j;
            j = 0;
        }
    }

    return needle[j] == '\0' ? i - j : -1;
}