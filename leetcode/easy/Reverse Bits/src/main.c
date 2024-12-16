#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const uint32_t n = 0b00000010100101000001111010011100;

    const uint32_t expected = 0b00111001011110000010100101000000;

    const uint32_t res = reverseBits( n );

    TEST_ASSERT_EQUAL_UINT32( expected, res );
}

void Test2( void ) {
    const uint32_t n = 0b11111111111111111111111111111101;

    const uint32_t expected = 0b10111111111111111111111111111111;

    const uint32_t res = reverseBits( n );

    TEST_ASSERT_EQUAL_UINT32( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );

    return UNITY_END();
}
