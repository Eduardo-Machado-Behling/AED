#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CLASSES 100000

#define GET_PARENT( i ) ( ( i - 1 ) >> 1 )
#define GET_LEFT( i ) ( ( i << 1 ) + 1 )  // 2i + 1
#define GET_RIGHT( i ) ( ( i + 1 ) << 1 ) // 2i + 2 == 2(i + 1)

struct entry {
    int* ref;
    double delta;
};

// #define __DEBUG

#ifdef __DEBUG
#include <math.h>
#include <stdio.h>
int printHeapTree( struct entry heap[], int used ) {
    if ( used == 0 ) {
        printf( "Heap is empty.\n" );
        return 1;
    }

    int height     = (int) ceil( log2( used + 1 ) ); // Height of the tree
    int levelWidth = (int) pow( 2, height - 1 );     // Width of the last level

    for ( int i = 0; i < height; i++ ) {
        int levelStart = (int) pow( 2, i ) - 1;                       // Start index of the current level
        int levelEnd   = (int) fmin( pow( 2, i + 1 ) - 2, used - 1 ); // End index of the current level
        int spaces     = levelWidth;

        for ( int j = levelStart; j <= levelEnd; j++ ) {
            // Print leading spaces
            for ( int k = 0; k < spaces; k++ ) {
                printf( "  " );
            }

            printf( "%.6f", heap[j].delta ); // Print the delta value

            // Print intermediate spaces
            int interSpaces = 2 * spaces - 2;
            if ( j < levelEnd ) { // Only print if not the last element on the level
                for ( int k = 0; k < interSpaces; k++ ) {
                    printf( "  " );
                }
            }
        }
        printf( "\n" );
        levelWidth /= 2; // Reduce width for the next level
    }

    return 1;
}
#endif

void swap( struct entry* a, struct entry* b ) {
    struct entry temp = *a;
    *a                = *b;
    *b                = temp;
}

void heapify( struct entry heap[MAX_CLASSES], int used, int i ) {
    int largest = i;
    do {
        i = largest;
#ifdef __DEBUG
        printf( "i = %d (parent=%d, %s)\n", i, GET_PARENT( i ), i % 2 == 1 ? "left" : "right" );
        printHeapTree( heap, used );
#endif

        int left  = GET_LEFT( i );
        int right = GET_RIGHT( i );

        if ( right < used && ( heap[right].delta > heap[largest].delta ) )
            largest = right;

        if ( left < used && ( heap[left].delta > heap[largest].delta ) )
            largest = left;

        if ( largest != i ) {
            swap( &heap[i], &heap[largest] );
        }

#ifdef __DEBUG
        printf( "\n" );
        printHeapTree( heap, used );
        printf( "\n" );
#endif
    } while ( largest != i );

#ifdef __DEBUG
    printf( "=====================\n\n" );
#endif
}

void heapSort( struct entry heap[MAX_CLASSES], int used ) {
    for ( int i = GET_PARENT( used ); i >= 0; i-- ) {
        heapify( heap, used, i );
    }
}

double maxAverageRatio( int** classes, int classesSize, int* classesColSize,
                        int extraStudents ) {
    struct entry heap[MAX_CLASSES] = { 0 };
    int used                       = 0;
    double res                     = 0;

    for ( int i = 0; i < classesSize; i++ ) {
        double change = (double) ( classes[i][0] ) / ( classes[i][1] );
        res += change;
        heap[used].ref     = classes[i];
        heap[used++].delta = (double) ( classes[i][0] + 1 ) / ( classes[i][1] + 1 ) - change;
    }
#ifdef __DEBUG
    printf( "SORTING:\n" );
#endif
    heapSort( heap, used );

#ifdef __DEBUG
    printf( "EXTRAS:\n" );
#endif
    for ( int i = 0; i < extraStudents; i++ ) {
        double max = heap[0].delta;
        res += max;
        int* ref      = heap[0].ref;
        double change = (double) ++ref[0] / ++ref[1];
        heap[0].delta = ( (double) ( ref[0] + 1 ) / ( ref[1] + 1 ) ) - change;

        if ( i < extraStudents - 1 ) {
            heapify( heap, used, 0 );
        }
    }

    return res / classesSize;
}
