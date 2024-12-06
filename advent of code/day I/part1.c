#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) )

int Compare( const void* a, const void* b ) {
    return *(int*) a - *(int*) b;
}

int GetDistance( int* listA, int* listB, int listSize ) {
    qsort( listA, listSize, sizeof( int ), Compare );
    qsort( listB, listSize, sizeof( int ), Compare );

    int distance = 0;
    for ( size_t i = 0; i < listSize; i++ ) {
        distance += abs( listB[i] - listA[i] );
    }

    return distance;
}

typedef struct DArray {
    int* data;
    int used;
    int size;
} DArray_t;

void destroy_DArray( DArray_t** );

DArray_t* create_DArray( int initial_size ) {
    DArray_t* darray = malloc( sizeof( DArray_t ) );
    if ( !darray ) {
        return NULL;
    }

    darray->used = 0;
    darray->size = initial_size;
    darray->data = malloc( darray->size * sizeof( int ) );
    if ( !darray->data ) {
        destroy_DArray( &darray );
        return NULL;
    }

    return darray;
}

bool DArray_push( DArray_t* darray, int value ) {
    if ( darray->used == darray->size ) {
        int newSize = darray->size + MIN( darray->size * 2, 200 );

        void* newMem = realloc( darray->data, newSize * sizeof( int ) );
        if ( !newMem ) {
            return false;
        }

        darray->data = newMem;
        darray->size = newSize;
    }

    darray->data[darray->used++] = value;

    return true;
}

void destroy_DArray( DArray_t** darray ) {
    if ( !darray ) {
        return;
    }

    DArray_t* da = *darray;
    *darray      = NULL;

    if ( !da ) {
        return;
    }

    if ( da->data ) {
        free( da->data );
    }
    free( da );
}

int main() {
    char buff[BUFSIZ] = { 0 };
    FILE* f           = fopen( "input.txt", "r" );

    DArray_t* darray[2] = { [0] = create_DArray( 20 ), [1] = create_DArray( 20 ) };
    if ( !darray[0] || !darray[1] ) {
        return EXIT_FAILURE;
    }

    for ( int i = 0; fgets( buff, BUFSIZ, f ); i++ ) {
        char* end = NULL;
        DArray_push( darray[0], strtol( buff, &end, 10 ) );
        for ( ; !isgraph( *end ); end++ )
            ;

        DArray_push( darray[1], strtol( end, &end, 10 ) );
    }

    printf( "%d\n", GetDistance( darray[0]->data, darray[1]->data, MIN( darray[0]->used, darray[1]->used ) ) );

    if ( fclose( f ) )
        return EXIT_FAILURE;
    destroy_DArray( &darray[0] );
    destroy_DArray( &darray[1] );
}