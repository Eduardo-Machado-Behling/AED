#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10000

struct entry_t {
    int diff;
    int profit;
};

struct array_t {
    int size;
    int elemSize;
    uint8_t* data;
};

struct span_t {
    int begin;
    int end;
};

typedef bool ( *f_predicate )( const void* a, const void* b );

int compare( const struct entry_t* a, const struct entry_t* b ) {
    return a->profit > b->profit;
}

void swap( void* a, void* b, int elemSize ) {
    if ( a == b )
        return;

    uint8_t* aa = a;
    uint8_t* bb = b;

    for ( int i = 0; i < elemSize; i++ ) {
        aa[i] = aa[i] ^ bb[i];
        bb[i] = aa[i] ^ bb[i];
        aa[i] = aa[i] ^ bb[i];
    }
}

void quickSort_r( struct array_t* a, void* pivot_value, struct span_t sp, f_predicate pred ) {
    const int mid = ( sp.begin + sp.end ) / 2 * a->elemSize;
    const int end = sp.end * a->elemSize;

    int i = sp.begin * a->elemSize;
    int j = end;

    memcpy( pivot_value, a->data + mid, a->elemSize );

    do {
        while ( i <= end && pred( a->data + i, pivot_value ) )
            i += a->elemSize;
        while ( j >= 0 && pred( pivot_value, a->data + j ) )
            j -= a->elemSize;

        if ( i <= j ) {
            swap( a->data + i, a->data + j, a->elemSize );
            i += a->elemSize;
            j -= a->elemSize;
        }
    } while ( i <= j );

    j /= a->elemSize;
    i /= a->elemSize;
    if ( sp.begin < j ) {
        quickSort_r( a, pivot_value, ( struct span_t ){ .begin = sp.begin, .end = j }, pred );
    }
    if ( i < sp.end ) {
        quickSort_r( a, pivot_value, ( struct span_t ){ .begin = i, .end = sp.end }, pred );
    }
}

void quickSort( struct array_t array, f_predicate pred ) {
    void* pivot_value = malloc( array.elemSize );
    quickSort_r( &array, pivot_value, ( struct span_t ){ .begin = 0, .end = array.size - 1 }, pred );
    free( pivot_value );
}

bool compareInt( const int* a, const int* b ) {
    return *a > *b;
}

// n == difficultySize == profitSize
// k == workerSize

// n + n(log n) + k(log k) + k + n
// n (1 + log n + 1) + k (log k + 1)
// n (log n) + k(log k)
int maxProfitAssignment( const int* difficulty, int difficultySize, const int* profit,
                         int profitSize, const int* worker, int workerSize ) {
    struct entry_t e[MAX_SIZE];

    // n
    for ( int i = 0; i < profitSize; i++ ) {
        e[i].diff   = difficulty[i];
        e[i].profit = profit[i];
    }

    // n (log n)
    quickSort( ( struct array_t ){ .data = (uint8_t*) e, .size = profitSize, .elemSize = sizeof( *e ) }, (f_predicate) compare );
    // k (log k)
    quickSort( ( struct array_t ){ .data = (uint8_t*) worker, .size = workerSize, .elemSize = sizeof( *worker ) }, (f_predicate)
                                                                                                                       compareInt );

    int sum = 0;
    int j   = 0;

    // k
    for ( int i = 0; i < workerSize; i++ ) {
        //* + n isn't quadratic because j is initialized outside the loop
        for ( ; j < profitSize; j++ ) {
            if ( e[j].diff <= worker[i] ) {
                sum += e[j].profit;
                break;
            }
        }
    }

    return sum;
}
