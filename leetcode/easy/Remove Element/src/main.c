#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    int a[] = { 3, 2, 2, 3 };
    int sa  = sizeof( a ) / sizeof( int );
    int val = 3;

    int expected[] = { 2, 2 };

    int res = removeElement( a, sa, val );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_2( void ) {
    int a[] = { 0, 1, 2, 2, 3, 0, 4, 2 };
    int sa  = sizeof( a ) / sizeof( int );
    int val = 2;

    int expected[] = { 0, 1, 4, 0, 3 };

    int res = removeElement( a, sa, val );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_3( void ) {
    int a[] = { 3, 3 };
    int sa  = sizeof( a ) / sizeof( int );
    int val = 3;

    int expected[] = { 0, 1, 4, 0, 3 };

    int res = removeElement( a, sa, val );

    TEST_ASSERT_EQUAL_INT( res, 0 );
}

void test_4( void ) {
    int a[] = { 0, 4, 4, 0, 4, 4, 4, 0, 2 };
    int sa  = sizeof( a ) / sizeof( int );
    int val = 4;

    int expected[] = { 0, 2, 0, 0 };

    int res = removeElement( a, sa, val );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_5( void ) {
    int a[] = { 4, 5 };
    int sa  = sizeof( a ) / sizeof( int );
    int val = 5;

    int expected[] = { 4 };

    int res = removeElement( a, sa, val );

    TEST_ASSERT_EQUAL_INT( sizeof( expected ) / sizeof( int ), res );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );
    RUN_TEST( test_3 );
    RUN_TEST( test_4 );
    RUN_TEST( test_5 );

    return UNITY_END();
}
