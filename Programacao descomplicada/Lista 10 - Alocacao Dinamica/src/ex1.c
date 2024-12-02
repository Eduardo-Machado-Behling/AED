#include <stdio.h>
#include <stdlib.h>

int main( void ) {
    int* array = malloc( 5 * sizeof( int ) );
    for ( size_t i = 0; i < 5; i++ ) {
        int ret = 0;
        while ( ret != 1 )
            ret = scanf( "%d", array + i );
    }

    for ( size_t i = 0; i < 5; i++ ) {
        printf( "[%lu]: %d\n", i, array[i] );
    }

    free( array );
    return EXIT_SUCCESS;
}
