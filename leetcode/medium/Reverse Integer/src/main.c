#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int input    = 123;
    const int expected = 321;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int input    = -123;
    const int expected = -321;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int input    = 120;
    const int expected = 21;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test4( void ) {
    const int input    = 1534236469;
    const int expected = 0;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test5( void ) {
    const int input    = -2147483648;
    const int expected = 0;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test6( void ) {
    const int input    = 1563847412;
    const int expected = 0;

    const int res = reverse( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );
    RUN_TEST( Test5 );
    RUN_TEST( Test6 );

    return UNITY_END();
}
