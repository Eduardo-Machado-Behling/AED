#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void eraseMatrix( char** m, int rows ) {
    if ( !m )
        return;

    for ( size_t i = 0; i < rows; i++ ) {
        if ( m[i] )
            free( m[i] );
    }
    free( m );
}

char** makeMatrix( const char* m, int rows, int cols ) {
    char** mat = malloc( sizeof( char* ) * rows );
    if ( !mat )
        return NULL;

    for ( size_t i = 0; i < rows; i++ ) {
        mat[i] = malloc( sizeof( char* ) * cols );
        if ( !mat[i] ) {
            eraseMatrix( mat, i );
            return NULL;
        }
        for ( size_t j = 0; j < cols; j++ ) {
            mat[i][j] = m[i * cols + j];
        }
    }

    return mat;
}

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    char board[] = { '5', '3', '.', '.', '7', '.', '.', '.', '.', '6', '.', '.', '1', '9', '5', '.', '.', '.', '.', '9', '8', '.', '.', '.', '.', '6', '.', '8', '.', '.', '.', '6', '.', '.', '.', '3', '4', '.', '.', '8', '.', '3', '.', '.', '1', '7', '.', '.', '.', '2', '.', '.', '.', '6', '.', '6', '.', '.', '.', '.', '2', '8', '.', '.', '.', '.', '4', '1', '9', '.', '.', '5', '.', '.', '.', '.', '8', '.', '.', '7', '9' };
    int rows     = 9;

    char** m = makeMatrix( board, rows, rows );

    TEST_ASSERT_TRUE( isValidSudoku( m, rows, &rows ) );
}

void test_2( void ) {

    char board[] = { '8', '3', '.', '.', '7', '.', '.', '.', '.', '6', '.', '.', '1', '9', '5', '.', '.', '.', '.', '9', '8', '.', '.', '.', '.', '6', '.', '8', '.', '.', '.', '6', '.', '.', '.', '3', '4', '.', '.', '8', '.', '3', '.', '.', '1', '7', '.', '.', '.', '2', '.', '.', '.', '6', '.', '6', '.', '.', '.', '.', '2', '8', '.', '.', '.', '.', '4', '1', '9', '.', '.', '5', '.', '.', '.', '.', '8', '.', '.', '7', '9' };
    int rows     = 9;

    char** m = makeMatrix( board, rows, rows );

    TEST_ASSERT_FALSE( isValidSudoku( m, rows, &rows ) );
}

void test_3( void ) {

    char board[] = { '.', '.', '.', '.', '5', '.', '.', '1', '.', '.', '4', '.', '3', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '3', '.', '.', '1', '8', '.', '.', '.', '.', '.', '.', '2', '.', '.', '.', '2', '.', '7', '.', '.', '.', '.', '.', '1', '5', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '2', '.', '.', '.', '.', '2', '.', '9', '.', '.', '.', '.', '.', '.', '.', '4', '.', '.', '.', '.', '.', '.' };
    int rows     = 9;

    char** m = makeMatrix( board, rows, rows );

    TEST_ASSERT_FALSE( isValidSudoku( m, rows, &rows ) );
}
int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );
    RUN_TEST( test_3 );

    return UNITY_END();
}
