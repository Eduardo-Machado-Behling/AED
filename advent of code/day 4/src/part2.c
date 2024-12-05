#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

struct Vec2d {
    int x, y;
};

int SearchString( dArray_t* matrix, struct Vec2d pos ) {
    for ( int i = 0; i < 4; i++ ) {
        int x = i % 2;
        if ( x == 0 ) {
            x = -1;
        }
        int y = i / 2;
        if ( y == 0 ) {
            y = -1;
        }

        dArray_t** row = DArrayAt( matrix, pos.y );
        if ( !row ) {
            return 0;
        }
        char* ch = DArrayAt( *row, pos.x );
        if ( !ch ) {
            return 0;
        }

        if ( *ch != 'A' ) {
            return 0;
        }

        row = DArrayAt( matrix, pos.y + y );
        if ( !row ) {
            return 0;
        }
        ch = DArrayAt( *row, pos.x + x );
        if ( !ch ) {
            return 0;
        }

        if ( *ch == 'M' ) {
            dArray_t** row = DArrayAt( matrix, pos.y - y );
            if ( !row ) {
                return 0;
            }
            char* ch = DArrayAt( *row, pos.x - x );
            if ( !ch ) {
                return 0;
            }

            if ( *ch != 'S' ) {
                return 0;
            }
        } else if ( *ch == 'S' ) {
            dArray_t** row = DArrayAt( matrix, pos.y - y );
            if ( !row ) {
                return 0;
            }
            char* ch = DArrayAt( *row, pos.x - x );
            if ( !ch ) {
                return 0;
            }

            if ( *ch != 'M' ) {
                return 0;
            }
        } else {
            return 0;
        }
    }

    return 1;
}

int main( void ) {
    FILE* in = fopen( "input.txt", "r" );
    if ( !in ) {
        fprintf( stderr, "input file wasn't open for the following reason: \"%s\"\n",
                 strerror( errno ) );
        return 1;
    }

    dArray_t* matrix = CreateDArray( 20, sizeof( dArray_t* ), (f_destructor) DestroyDArray );
    dArray_t* row    = CreateDArray( 20, sizeof( char ), NULL );
    DArrayPush( matrix, &row );
    for ( int ch = 0; ch != EOF; ) {
        ch = fgetc( in );
        if ( isgraph( ch ) ) {
            DArrayPush( row, &ch );
        } else if ( ch == '\n' ) {
            row = CreateDArray( 20, sizeof( char ), NULL );
            DArrayPush( matrix, &row );
        }
    }

    int res = 0;
    for ( int i = 1; i < DArrayUsed( matrix ); i++ ) {
        dArray_t** row = DArrayAt( matrix, i );
        for ( int j = 0; j < DArrayUsed( *row ); j++ ) {
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i } );
        }
    }

    printf( "res %d\n", res );
    fclose( in );
}
