#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    int a[] = { 1, 1, 1, 2, 2, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1, 2, 2, 3 };
    int expected_size = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected_size );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_2( void ) {
    int a[] = { 0, 0, 1, 1, 1, 1, 2, 3, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 0, 0, 1, 1, 2, 3, 3 };
    int expected_size = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected_size );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_3( void ) {
    int a[] = { 1, 1, 1 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1 };
    int expected_size = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected_size );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

void test_4( void ) {
    int a[] = { 1, 1, 1, 2, 2, 2, 3, 3 };
    int sa  = sizeof( a ) / sizeof( int );

    int expected[]    = { 1, 1, 2, 2, 3, 3 };
    int expected_size = sizeof( expected ) / sizeof( int );

    int res = removeDuplicates( a, sa );

    TEST_ASSERT_EQUAL_INT( res, expected_size );
    TEST_ASSERT_EQUAL_INT32_ARRAY( expected, a, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );
    RUN_TEST( test_3 );
    RUN_TEST( test_4 );

    return UNITY_END();
}
