#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    const char* str = "abcabcbb";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void test_2( void ) {
    const char* str = "bbbbb";
    int expected    = 1;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void test_3( void ) {
    const char* str = "pwwkew";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void test_4( void ) {
    const char* str = " ";
    int expected    = 1;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
}

void test_5( void ) {
    const char* str = "dvdf";
    int expected    = 3;

    TEST_ASSERT_EQUAL_INT( expected, lengthOfLongestSubstring( str ) );
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
