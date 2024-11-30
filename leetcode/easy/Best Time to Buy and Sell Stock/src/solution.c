#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct entry_t {
    int val;
    int index;
};

struct stack_t {
    struct entry_t* data;
    int i;
};

int Compare( const void* a, const void* b ) {
    return ( (struct entry_t*) b )->val - ( (struct entry_t*) a )->val;
}

int maxProfit( const int* prices, int pricesSize ) {
    struct stack_t stack = {
        .data = malloc( pricesSize * sizeof( struct entry_t ) ),
        .i    = 0
    };
    if ( !stack.data ) {
        return 0;
    }

    for ( int i = 0; i < pricesSize; i++ ) {
        stack.data[i].val   = prices[i];
        stack.data[i].index = i;
    }

    qsort( stack.data, pricesSize, sizeof( struct entry_t ), Compare );

    int maxProfit = 0;
    for ( int i = 0; i < pricesSize - 1; i++ ) {
        for ( ; stack.data[stack.i].index <= i; stack.i++ ) {
            ;
        }

        int profit = stack.data[stack.i].val - prices[i];
        if ( profit > maxProfit ) {
            maxProfit = profit;
        }
    }

    return maxProfit;
}