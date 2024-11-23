#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    const char* str = "42";
    int expected    = 42;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_2( void ) {
    const char* str = " -042";
    int expected    = -42;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_3( void ) {
    const char* str = "1337c0d3";
    int expected    = 1337;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_4( void ) {
    const char* str = "0-1";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_5( void ) {
    const char* str = "words and 987";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_6( void ) {
    const char* str = "+-12";
    int expected    = 0;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_7( void ) {
    const char* str = "-6147483648";
    int expected    = -2147483648;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_8( void ) {
    const char* str = "  0000000000012345678";
    int expected    = 12345678;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_9( void ) {
    const char* str = "-2147483648";
    int expected    = INT32_MIN;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_10( void ) {
    const char* str = "-000000000000000000000000000000000000000000000000001";
    int expected    = -1;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_11( void ) {
    const char* str = "-2147483647";
    int expected    = -2147483647;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
}

void test_12( void ) {
    const char* str = "1095502006p8";
    int expected    = 1095502006;

    TEST_ASSERT_EQUAL_INT( expected, myAtoi( str ) );
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
    RUN_TEST( test_8 );
    RUN_TEST( test_9 );
    RUN_TEST( test_10 );
    RUN_TEST( test_11 );
    RUN_TEST( test_12 );

    return UNITY_END();
}
