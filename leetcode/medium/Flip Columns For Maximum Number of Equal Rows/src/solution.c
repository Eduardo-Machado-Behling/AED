#include "solution.h"

#include <stdlib.h>
#include <string.h>

struct idPattern {
    int* pattern;
    int size;
    int freq;
};

struct idPatternArray {
    struct idPattern* array;
    int used;
};

void parsePattern( int* row, int row_size, int* pattern, int* patternSize ) {
    *patternSize = 0;
    int i        = 1;
    for ( ; i <= row_size; i++ ) {
        if ( i == row_size || row[i - 1] != row[i] ) {
            int offset                  = *patternSize == 0 ? 0 : pattern[*patternSize - 1];
            int v                       = i - abs( offset );
            pattern[( *patternSize )++] = v;
        }
    }
}

int cmpPattern( int* pa, int sizea, int* pb, int sizeb ) {
    if ( sizea != sizeb )
        return 0;

    for ( size_t i = 0; i < sizea; i++ ) {
        if ( pa[i] != pb[i] )
            return 0;
    }

    return 1;
}

int maxEqualRowsAfterFlips( int** matrix, int matrixSize, int* matrixColSize ) {
    if ( *matrixColSize == 1 )
        return matrixSize;

    struct idPatternArray patterns;
    patterns.array = malloc( matrixSize * sizeof( struct idPattern ) );
    patterns.used  = 0;

    int* pattern = malloc( sizeof( int ) * *matrixColSize );
    int size;
    for ( int i = 0; i < matrixSize; i++ ) {
        struct idPattern* pPattern = NULL;

        parsePattern( matrix[i], *matrixColSize, pattern, &size );

        for ( int j = 0; j < patterns.used; j++ ) {
            struct idPattern* compPattern = patterns.array + j;
            if ( cmpPattern( pattern, size, compPattern->pattern, compPattern->size ) ) {
                pPattern = patterns.array + j;
                break;
            }
        }

        if ( !pPattern ) {
            pPattern          = patterns.array + patterns.used++;
            pPattern->pattern = malloc( sizeof( int ) * size );
            memcpy( pPattern->pattern, pattern, sizeof( int ) * size );

            pPattern->size = size;
            pPattern->freq = 1;
        } else {
            pPattern->freq++;
        }
    }

    int max = -1;
    for ( int i = 0; i < patterns.used; i++ ) {
        if ( patterns.array[i].freq > max )
            max = patterns.array[i].freq;
        free( patterns.array[i].pattern );
    }

    free( patterns.array );
    free( pattern );
    return max;
}