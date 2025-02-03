#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int input[]   = { 1, 4, 3, 3, 2 };
    const int inputSize = sizeof( input ) / sizeof( *input );
    const int expected  = 2;

    const int res = longestMonotonicSubarray( input, inputSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int input[]   = { 3, 3, 3, 3 };
    const int inputSize = sizeof( input ) / sizeof( *input );
    const int expected  = 1;

    const int res = longestMonotonicSubarray( input, inputSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int input[]   = { 3, 2, 1 };
    const int inputSize = sizeof( input ) / sizeof( *input );
    const int expected  = 3;

    const int res = longestMonotonicSubarray( input, inputSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test4( void ) {
    const int input[]   = { 1, 5, 2, 7 };
    const int inputSize = sizeof( input ) / sizeof( *input );
    const int expected  = 2;

    const int res = longestMonotonicSubarray( input, inputSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );

    return UNITY_END();
}
