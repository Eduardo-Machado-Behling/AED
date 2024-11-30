#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    int a[]        = { 1, 2, 3, 4, 5, 6, 7 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 3;
    int expected[] = { 5, 6, 7, 1, 2, 3, 4 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test2( void ) {
    int a[]        = { -1, -100, 3, 99 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 2;
    int expected[] = { 3, 99, -1, -100 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test3( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 0;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test4( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 1;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test5( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 2;
    int expected[] = { 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test6( void ) {
    int a[]        = { 1, 2 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 3;
    int expected[] = { 2, 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test7( void ) {
    int a[]        = { 1, 2, 3, 4, 5, 6 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 4;
    int expected[] = { 3, 4, 5, 6, 1, 2 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test8( void ) {
    int a[]        = { 1, 2 };
    int sa         = sizeof( a ) / sizeof( int );
    int k          = 1;
    int expected[] = { 2, 1 };

    rotate( a, sa, k );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );
    RUN_TEST( Test5 );
    RUN_TEST( Test6 );
    RUN_TEST( Test7 );
    RUN_TEST( Test8 );

    return UNITY_END();
}
