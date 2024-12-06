#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"
#include "hashmap.h"

int HashFunction( void* key ) {
    return *(int*) key;
}

int GetMidElementIfValid( hashMap_t* rules, dArray_t* sequence ) {
    static const hashMap_t* count = NULL;
    if ( count ) {
        HashMapClear( count );
    } else {
        count = CreateHashMap( 40, 10, sizeof( int ), sizeof( int ), (f_hashfunc) HashFunction );
    }

    for ( size_t i = 0; i < DArrayUsed( sequence ); i++ ) {
        int* num              = DArrayAt( sequence, i );
        dArray_t** afterRules = HashMapGet( rules, num );

        int* countNumHash = HashMapGet( count, num );
        if ( !countNumHash ) {
            int one = 1;
            HashMapSet( count, num, &one );
        } else {
            ( *countNumHash )++;
        }

        if ( !afterRules ) {
            continue;
        }

        for ( size_t j = 0; j < DArrayUsed( *afterRules ); j++ ) {
            int* ruleNum  = DArrayAt( *afterRules, j );
            int* countNum = HashMapGet( count, ruleNum );
            if ( countNum && *countNum > 0 ) {
                return 0;
            }
        }
    }

    int* res = DArrayAt( sequence, DArrayUsed( sequence ) / 2 );
    return *res;
}

int Compare( void* rulesPtr, const void* aPtr, const void* bPtr ) {
    hashMap_t* rules = rulesPtr;

    const int* a = aPtr;
    const int* b = bPtr;

    dArray_t** da = HashMapGet( rules, (void*) b );
    if ( !da )
        return 1;

    for ( size_t i = 0; i < DArrayUsed( *da ); i++ ) {
        int* num = DArrayAt( *da, i );

        if ( *a == *num ) {
            return -1;
        }
    }

    return 1;
}

int GetMidUpdating( hashMap_t* rules, dArray_t* sequence ) {
    int* arr = DArrayAt( sequence, 0 );
    qsort_s( arr, DArrayUsed( sequence ), sizeof( int ), Compare, rules );

    return arr[DArrayUsed( sequence ) / 2];
}

int main( void ) {
    FILE* in = fopen( "input.txt", "r" );
    if ( !in ) {
        fprintf( stderr, "input file wasn't open for the following reason: \"%s\"\n",
                 strerror( errno ) );
        return 1;
    }

    hashMap_t* hashmap = CreateHashMap( 64, 20, sizeof( int ), sizeof( dArray_t* ), (f_hashfunc) HashFunction );
    char buffer[BUFSIZ];
    while ( fgets( buffer, BUFSIZ, in )[0] != '\n' ) {
        char* line = buffer;
        int key    = strtol( line, &line, 10 );
        int val    = strtol( ++line, &line, 10 );

        dArray_t** valList = HashMapGet( hashmap, &key );
        if ( !valList ) {
            dArray_t* newList = CreateDArray( 20, sizeof( int ), NULL );
            HashMapSet( hashmap, &key, &newList );
            valList = &newList;
        }
        DArrayPush( *valList, &val );
    }

    dArray_t* sequences = CreateDArray( 20, sizeof( dArray_t* ), (f_destructor) DestroyDArray );
    while ( fgets( buffer, BUFSIZ, in ) ) {
        dArray_t* sequence = CreateDArray( 20, sizeof( int ), NULL );

        char* line = buffer;
        while ( 1 ) {
            int val = strtol( line, &line, 10 );
            DArrayPush( sequence, &val );
            if ( *line == '\0' || *line++ == '\n' ) {
                break;
            }
        }

        DArrayPush( sequences, &sequence );
    }

    long res = 0;
    for ( size_t i = 0; i < DArrayUsed( sequences ); i++ ) {
        dArray_t** da = DArrayAt( sequences, i );
        if ( !GetMidElementIfValid( hashmap, *da ) ) {
            res += GetMidUpdating( hashmap, *da );
            for ( size_t j = 0; j < DArrayUsed( *da ); j++ ) {
            }
        }
    }

    printf( "res %d\n", res );
    fclose( in );
}
