#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int n = 11;

    const int expected = 3;

    const int res = hammingWeight( n );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int n = 128;

    const int expected = 1;

    const int res = hammingWeight( n );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int n = 2147483645;

    const int expected = 30;

    const int res = hammingWeight( n );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
