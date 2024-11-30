#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    int a[] = { 1, 2, 3, 0, 0, 0 };
    int sa  = sizeof( a ) / sizeof( int );
    int b[] = { 2, 5, 6 };
    int sb  = sizeof( b ) / sizeof( int );

    int expected[] = { 1, 2, 2, 3, 5, 6 };
    int m          = 3;
    int n          = 3;

    merge( a, sa, m, b, sb, n );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test2( void ) {
    int a[]        = { 1 };
    int sa         = sizeof( a ) / sizeof( int );
    int b[]        = { 0 };
    int sb         = sizeof( b ) / sizeof( int );
    int expected[] = { 1 };
    int m          = 1;
    int n          = 0;

    merge( a, sa, m, b, sb, n );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test3( void ) {
    int a[]        = { 0 };
    int sa         = sizeof( a ) / sizeof( int );
    int b[]        = { 1 };
    int sb         = sizeof( b ) / sizeof( int );
    int expected[] = { 1 };
    int m          = 0;
    int n          = 1;

    merge( a, sa, m, b, sb, n );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

void Test4( void ) {
    int a[]        = { 1, 2, 3, 0, 0, 0 };
    int sa         = sizeof( a ) / sizeof( int );
    int b[]        = { 2, 5, 6 };
    int sb         = sizeof( b ) / sizeof( int );
    int expected[] = { 1, 2, 2, 3, 5, 6 };
    int m          = 3;
    int n          = 3;

    merge( a, sa, m, b, sb, n );

    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, sa );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );

    return UNITY_END();
}
