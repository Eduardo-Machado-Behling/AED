#include "unity.h"

#include "solution.h"

#include <stdlib.h>

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int input       = 3749;
    const char expected[] = "MMMDCCXLIX";

    char* res = intToRoman( input );

    TEST_ASSERT_EQUAL_STRING( res, expected );
    free( res );
}

void Test2( void ) {
    const int input       = 58;
    const char expected[] = "LVIII";

    char* res = intToRoman( input );

    TEST_ASSERT_EQUAL_STRING( res, expected );
    free( res );
}

void Test3( void ) {
    const int input       = 1994;
    const char expected[] = "MCMXCIV";

    char* res = intToRoman( input );

    TEST_ASSERT_EQUAL_STRING( res, expected );
    free( res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
