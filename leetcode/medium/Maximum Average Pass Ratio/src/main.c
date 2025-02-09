#include "unity.h"

#include "solution.h"
#include <stdlib.h>
#include <string.h>

void setUp( void ) {
}

void tearDown( void ) {
}

int** makeMatrix( int classes[][2], int classSize ) {
    int** matrix = malloc( sizeof( int* ) * classSize );

    for ( int i = 0; i < classSize; i++ ) {
        matrix[i] = malloc( sizeof( int ) * 2 );
        memcpy( matrix[i], classes[i], sizeof( int ) * 2 );
    }

    return matrix;
}

void Test1( void ) {
    int classes[][2]        = { { 1, 2 }, { 3, 5 }, { 2, 2 } };
    const int classesSize   = sizeof( classes ) / sizeof( *classes );
    const int extraStudents = 2;
    int classesColSize      = 2;
    int** matrix            = makeMatrix( classes, classesSize );

    const double expected = 0.78333;

    const double res =
        maxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );

    TEST_ASSERT_EQUAL_DOUBLE( expected, res );
    for ( size_t i = 0; i < classesSize; i++ ) {
        free( matrix[i] );
    }
    free( matrix );
}

void Test2( void ) {
    int classes[][2]        = { { 2, 4 }, { 3, 9 }, { 4, 5 }, { 2, 10 } };
    const int classesSize   = sizeof( classes ) / sizeof( *classes );
    const int extraStudents = 4;
    int classesColSize      = 2;
    int** matrix            = makeMatrix( classes, classesSize );

    const double expected = 0.53485;

    const double res =
        maxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );

    TEST_ASSERT_EQUAL_DOUBLE( expected, res );
    for ( size_t i = 0; i < classesSize; i++ ) {
        free( matrix[i] );
    }
    free( matrix );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );

    return UNITY_END();
}
