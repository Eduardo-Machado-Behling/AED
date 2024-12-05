#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const char* str1 = "abc";
    const char* str2 = "ad";

    const bool expected = true;
    const bool res      = canMakeSubsequence( str1, str2 );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const char* str1 = "zc";
    const char* str2 = "ad";

    const bool expected = true;
    const bool res      = canMakeSubsequence( str1, str2 );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const char* str1 = "ab";
    const char* str2 = "d";

    const bool expected = false;
    const bool res      = canMakeSubsequence( str1, str2 );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test4( void ) {
    const char* str1 = "c";
    const char* str2 = "b";

    const bool expected = false;
    const bool res      = canMakeSubsequence( str1, str2 );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test5( void ) {
    const char* str1 = "eao";
    const char* str2 = "ofa";

    const bool expected = false;
    const bool res      = canMakeSubsequence( str1, str2 );

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
