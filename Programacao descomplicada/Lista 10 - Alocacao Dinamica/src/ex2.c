#include <stdio.h>
#include <stdlib.h>

int main( void ) {
    int tam = 0;
    int ret = 0;

    printf( "Entre o tamanho do array: " );
    while ( ret == 0 ) {
        ret = scanf( "%d", &tam );
    }
    int* array = malloc( tam * sizeof( int ) );
    for ( size_t i = 0; i < tam; i++ ) {
        ret = 0;
        while ( ret != 1 )
            ret = scanf( "%d", array + i );
    }

    for ( size_t i = 0; i < tam; i++ ) {
        printf( "[%lu]: %d\n", i, array[i] );
    }

    free( array );
    return EXIT_SUCCESS;
}
