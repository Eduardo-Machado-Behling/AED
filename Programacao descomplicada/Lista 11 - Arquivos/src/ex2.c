#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( void ) {
    FILE* f = NULL;
    char name[255];

    printf( "Entre o nome do arquivo: " );
    fgets( name, 255, stdin );
    *strchr( name, '\n' ) = '\0';

    f = fopen( name, "r" );
    if ( !f ) {
        fprintf( stderr, "Não foi possivel abrir o arquivo\n\tmotivo: %s\n", strerror( errno ) );
        return EXIT_FAILURE;
    }

    int ch    = 0;
    int count = 0;
    do {
        ch = fgetc( f );
        if ( ch == '\n' )
            count++;
    } while ( ch != EOF );

    printf( "%d linhas\n", count );

    return EXIT_SUCCESS;
}
