#include "solution.h"

#define BITS_SIZE_CONSTRAIN 32U

uint32_t reverseBits( uint32_t n ) {
    uint32_t reversed = 0;
    for ( uint8_t i = 0; i < BITS_SIZE_CONSTRAIN; i++ ) {
        reversed = ( reversed << 1U ) | ( n & 1U );
        n >>= 1U;
    }

    return reversed;
}