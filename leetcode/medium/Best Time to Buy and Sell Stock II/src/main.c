#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    int a[] = { 7, 1, 5, 3, 6, 4 };
    int sa  = sizeof( a ) / sizeof( int );

    const int expected = 7;

    int res = maxProfit( a, sa );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    int a[] = { 1, 2, 3, 4, 5 };
    int sa  = sizeof( a ) / sizeof( int );

    const int expected = 4;

    int res = maxProfit( a, sa );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    int a[] = { 7, 6, 4, 3, 1 };
    int sa  = sizeof( a ) / sizeof( int );

    const int expected = 0;

    int res = maxProfit( a, sa );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
