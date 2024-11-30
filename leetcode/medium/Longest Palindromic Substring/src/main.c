#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const char* str      = "babad";
    const char* expected = "bab";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test2( void ) {
    const char* str      = "cbbd";
    const char* expected = "bb";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test3( void ) {
    const char* str      = "ccc";
    const char* expected = "ccc";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test4( void ) {
    const char* str      = "aaaa";
    const char* expected = "aaaa";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test5( void ) {
    const char* str      = "a";
    const char* expected = "a";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test6( void ) {
    const char* str      = "caaaaa";
    const char* expected = "aaaaa";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
}

void Test7( void ) {
    const char* str      = "bb";
    const char* expected = "bb";

    char* res = longestPalindrome( str );

    TEST_ASSERT_EQUAL_STRING( expected, res );
    free( res );
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

    return UNITY_END();
}
