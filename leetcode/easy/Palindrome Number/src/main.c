#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int input = 121;

    const bool expected = true;

    const int res = isPalindrome( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int input = -121;

    const bool expected = false;

    const bool res = isPalindrome( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int input = 10;

    const bool expected = false;

    const bool res = isPalindrome( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test4( void ) {
    const int input = 0;

    const bool expected = true;

    const bool res = isPalindrome( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test5( void ) {
    const int input = 1001;

    const bool expected = true;

    const bool res = isPalindrome( input );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );
    RUN_TEST( Test4 );
    RUN_TEST( Test5 );

    return UNITY_END();
}
