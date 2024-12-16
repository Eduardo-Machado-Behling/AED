#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const char* haystack = "sadbutsad";
    const char* neddle   = "sad";

    const int expected = 0;

    const int res = strStr( haystack, neddle );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const char* haystack = "leetcode";
    const char* neddle   = "leeto";

    const int expected = -1;

    const int res = strStr( haystack, neddle );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const char* haystack = "mississippi";
    const char* neddle   = "issip";

    const int expected = 4;

    const int res = strStr( haystack, neddle );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
