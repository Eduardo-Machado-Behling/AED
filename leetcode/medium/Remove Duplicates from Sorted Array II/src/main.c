#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    int a[] = { 1, 1, 1, 2, 2, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1, 2, 2, 3 };
    int expectedSize = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expectedSize );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void Test2( void ) {
    int a[] = { 0, 0, 1, 1, 1, 1, 2, 3, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 0, 0, 1, 1, 2, 3, 3 };
    int expectedSize = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expectedSize );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void Test3( void ) {
    int a[] = { 1, 1, 1 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1 };
    int expectedSize = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expectedSize );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void Test4( void ) {
    int a[] = { 1, 1, 1, 2, 2, 2, 3, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1, 2, 2, 3, 3 };
    int expectedSize = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expectedSize );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );

    return UNITY_END();
}
