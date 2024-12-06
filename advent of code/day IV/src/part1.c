#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

struct Vec2d {
    int x, y;
};

int SearchString( dArray_t* matrix, struct Vec2d pos, struct Vec2d delta, const char* str, size_t strLen ) {
    for ( size_t i = 0; i < strLen; i++ ) {
        dArray_t** row = DArrayAt( matrix, pos.y );
        if ( !row ) {
            return 0;
        }

        char* currChar = (char*) DArrayAt( *row, pos.x );
        if ( !currChar || *currChar != str[i] ) {
            return 0;
        }

        pos.x += delta.x;
        pos.y += delta.y;
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

    const char str[] = "XMAS";
    int res          = 0;
    size_t strLen    = sizeof( str ) / sizeof( char ) - 1;
    for ( size_t i = 0; i < DArrayUsed( matrix ); i++ ) {
        dArray_t** row = DArrayAt( matrix, i );
        for ( size_t j = 0; j < DArrayUsed( *row ); j++ ) {
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = 0, .y = 1 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = 0, .y = -1 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = 1, .y = 0 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = -1, .y = 0 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = -1, .y = -1 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = 1, .y = 1 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = 1, .y = -1 }, str, strLen );
            res += SearchString( matrix, ( struct Vec2d ){ .x = j, .y = i }, ( struct Vec2d ){ .x = -1, .y = 1 }, str, strLen );
        }
    }

    printf( "res %d\n", res );
    fclose( in );
}
