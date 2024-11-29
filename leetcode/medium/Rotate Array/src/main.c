#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    int a[]        = { 1, 2, 3, 4, 5, 6, 7 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 3;
    int expected[] = { 5, 6, 7, 1, 2, 3, 4 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_2( void ) {
    int a[]        = { -1, -100, 3, 99 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 2;
    int expected[] = { 3, 99, -1, -100 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_3( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 0;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_4( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 1;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_5( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 2;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_6( void ) {
    int a[]        = { 1, 2 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 3;
    int expected[] = { 2, 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_7( void ) {
    int a[]        = { 1, 2, 3, 4, 5, 6 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 4;
    int expected[] = { 3, 4, 5, 6, 1, 2 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void test_8( void ) {
    int a[]        = { 1, 2 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 1;
    int expected[] = { 2, 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );
    RUN_TEST( test_3 );
    RUN_TEST( test_4 );
    RUN_TEST( test_5 );
    RUN_TEST( test_6 );
    RUN_TEST( test_7 );
    RUN_TEST( test_8 );

    return UNITY_END();
}
