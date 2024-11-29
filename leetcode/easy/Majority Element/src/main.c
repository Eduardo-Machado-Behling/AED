#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    int a[] = { 3, 1, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected = 3;

    int res = majorityElement( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected );
}

void test_2( void ) {
    int a[] = { 2, 2, 1, 1, 1, 2, 2 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected = 2;

    int res = majorityElement( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );

    return UNITY_END();
}
