#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CLASSES 100000

#define GET_PARENT( i ) ( ( i - 1 ) >> 1 )
#define GET_LEFT( i ) ( i << 1 + 1 )      // 2i + 1
#define GET_RIGHT( i ) ( ( i + 1 ) << 2 ) // 2i + 2 == 2(i + 1)

struct entry {
    int* ref;
    double delta;
};

void swap( struct entry* a, struct entry* b ) {
    struct entry temp = *a;
    *a                = *b;
    *b                = temp;
}

void heapify( struct entry heap[MAX_CLASSES], int used, bool reversed ) {
    for ( size_t i = used - 1; i > 0; i-- ) {
        size_t parent = GET_PARENT( i );
        if ( heap[parent].delta > heap[i].delta ^ reversed ) {
            swap( heap + parent, heap + i );
        }
    }
}

void heapSort( struct entry heap[MAX_CLASSES], int used ) {
    for ( size_t i = used - 1; i > 0; i-- ) {
        heapify( heap, i + 1, false );
        swap( heap, heap + i );
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

    heapSort( heap, used );

    for ( int i = 0; i < extraStudents; i++ ) {
        double max = heap[0].delta;
        res += max;
        int* ref      = heap[0].ref;
        double change = (double) ++ref[0] / ++ref[1];
        heap[0].delta = ( (double) ( ref[0] + 1 ) / ( ref[1] + 1 ) ) - change;
        swap( heap, heap + used - 1 );
        heapify( heap, used, true );
    }

    return res / classesSize;
}