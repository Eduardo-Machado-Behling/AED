#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int numRows = 5;

    const int expected[][5] = {
        { 1 }, { 1, 1 }, { 1, 2, 1 }, { 1, 3, 3, 1 }, { 1, 4, 6, 4, 1 }
    };

    int* columnsSize = NULL;
    int size         = 0;

    int** res = generate( numRows, &size, &columnsSize );

    for ( size_t i = 0; i < numRows; i++ ) {
        TEST_ASSERT_EQUAL_INT_ARRAY( expected[i], res[i], columnsSize[i] );
        free( res[i] );
    }

    free( columnsSize );
    free( (void*) res );
}

void Test2( void ) {
    const int numRows = 1;

    const int expected[][1] = { { 1 } };

    int* columnsSize = NULL;
    int size         = 0;

    int** res = generate( numRows, &size, &columnsSize );

    for ( size_t i = 0; i < numRows; i++ ) {
        TEST_ASSERT_EQUAL_INT_ARRAY( expected[i], res[i], columnsSize[i] );
        free( res[i] );
    }

    free( columnsSize );
    free( (void*) res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );

    return UNITY_END();
}
