#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const char* str = "42";
    int expected    = 42;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test2( void ) {
    const char* str = " -042";
    int expected    = -42;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test3( void ) {
    const char* str = "1337c0d3";
    int expected    = 1337;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test4( void ) {
    const char* str = "0-1";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test5( void ) {
    const char* str = "words and 987";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test6( void ) {
    const char* str = "+-12";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test7( void ) {
    const char* str = "-6147483648";
    int expected    = -2147483648;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test8( void ) {
    const char* str = "  0000000000012345678";
    int expected    = 12345678;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test9( void ) {
    const char* str = "-2147483648";
    int expected    = INT32_MIN;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test10( void ) {
    const char* str = "-000000000000000000000000000000000000000000000000001";
    int expected    = -1;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test11( void ) {
    const char* str = "-2147483647";
    int expected    = -2147483647;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void Test12( void ) {
    const char* str = "1095502006p8";
    int expected    = 1095502006;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
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
    RUN_TEST( Test8 );
    RUN_TEST( Test9 );
    RUN_TEST( Test10 );
    RUN_TEST( Test11 );
    RUN_TEST( Test12 );

    return UNITY_END();
}
