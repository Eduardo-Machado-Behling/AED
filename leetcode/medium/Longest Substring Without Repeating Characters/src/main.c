#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const char* str = "abcabcbb";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void Test2( void ) {
    const char* str = "bbbbb";
    int expected    = 1;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void Test3( void ) {
    const char* str = "pwwkew";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void Test4( void ) {
    const char* str = " ";
    int expected    = 1;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void Test5( void ) {
    const char* str = "dvdf";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
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
