#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    int nums[] = { 2,
                   7,
                   11, 15 };
    int returnSize;

    int* ret = twoSum( nums, sizeof( nums ) / sizeof( int ), 9, &returnSize );

    int expected[] = { 0, 1 };
    TEST_ASSERT_EQUAL_INT_ARRAY( expected, ret, returnSize );

    free( ret );
}

void Test2( void ) {
    int nums[] = { 3,
                   2,
                   4 };
    int returnSize;

    int* ret = twoSum( nums, sizeof( nums ) / sizeof( int ), 6, &returnSize );

    int expected[] = { 1, 2 };
    TEST_ASSERT_EQUAL_INT_ARRAY( expected, ret, returnSize );

    free( ret );
}

void Test3( void ) {
    int nums[] = { 3,
                   3 };
    int returnSize;

    int* ret = twoSum( nums, sizeof( nums ) / sizeof( int ), 6, &returnSize );

    int expected[] = { 0, 1 };
    TEST_ASSERT_EQUAL_INT_ARRAY( expected, ret, returnSize );

    free( ret );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
