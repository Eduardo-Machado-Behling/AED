#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int nums[]   = { 3, 4, 5, 1, 2 };
    const int numsSize = sizeof( nums ) / sizeof( *nums );

    const bool expected = true;

    const bool res = check( nums, numsSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int nums[]   = { 2, 1, 3, 4 };
    const int numsSize = sizeof( nums ) / sizeof( *nums );

    const bool expected = false;

    const bool res = check( nums, numsSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int nums[]   = { 1, 2, 3 };
    const int numsSize = sizeof( nums ) / sizeof( *nums );

    const bool expected = true;

    const bool res = check( nums, numsSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test4( void ) {
    const int nums[]   = { 8, 5, 4, 5, 1, 4, 5, 2, 2 };
    const int numsSize = sizeof( nums ) / sizeof( *nums );

    const bool expected = false;

    const bool res = check( nums, numsSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );

    return UNITY_END();
}
