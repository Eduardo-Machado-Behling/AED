#include <stdio.h>
#include <stdlib.h>

int main( void ) {
    FILE* f = fopen( "arq.txt", "w" );
    int ch  = 0;

    while ( ( ch = getchar() ) != '0' ) {
        fputc( ch, f );
    }

    fclose( f );
    return EXIT_SUCCESS;
}
