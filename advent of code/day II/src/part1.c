#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

int GetSafeLevelsAmount( dArray_t* report ) {
    int safeAmount = 0;

    for ( size_t i = 0; i < DArrayUsed( report ); i++ ) {
        dArray_t* level = *(dArray_t**) DArrayAt( report, i );
        int sign        = 0;
        int safe        = 1;
        for ( size_t j = 1; j < DArrayUsed( level ); j++ ) {
            int diff = *(int*) DArrayAt( level, j ) - *(int*) DArrayAt( level, j - 1 );
            if ( sign == 0 )
                sign = diff > 0 ? 1 : -1;

            if ( abs( diff ) > 3 || diff * sign <= 0 )
                safe = 0;
        }
        safeAmount += safe;
    }

    return safeAmount;
}

int main( void ) {
    FILE* in = fopen( "input.txt", "r" );
    if ( !in ) {
        fprintf( stderr, "input file wasn't open for the following reason: \"%s\"\n", strerror( errno ) );
        return 1;
    }

    dArray_t* report = CreateDArray( 20, sizeof( dArray_t* ), (f_destructor) DestroyDArray );
    if ( !report ) {
        fprintf( stderr, "Couldn't alloc memory for the lists\n" );
        return 1;
    }

    char buff[255];
    while ( fgets( buff, 255, in ) ) {
        char* line      = buff;
        dArray_t* level = CreateDArray( 20, sizeof( int ), NULL );
        DArrayPush( report, &level );
        while ( *line ) {
            int val = strtol( line, &line, 10 );
            DArrayPush( level, &val );
            for ( ; !isgraph( *line ) && *line != '\0'; line++ )
                ;
        }
    }

    printf( "%d", GetSafeLevelsAmount( report ) );

    fclose( in );
    DestroyDArray( &report );
}
