#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    const char* str      = "babad";
    const char* expected = "bab";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_2( void ) {
    const char* str      = "cbbd";
    const char* expected = "bb";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_3( void ) {
    const char* str      = "ccc";
    const char* expected = "ccc";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_4( void ) {
    const char* str      = "aaaa";
    const char* expected = "aaaa";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_5( void ) {
    const char* str      = "a";
    const char* expected = "a";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_6( void ) {
    const char* str      = "caaaaa";
    const char* expected = "aaaaa";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void test_7( void ) {
    const char* str      = "bb";
    const char* expected = "bb";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
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

    return UNITY_END();
}
