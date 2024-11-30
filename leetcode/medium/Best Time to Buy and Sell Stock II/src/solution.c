#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int maxProfit( const int* prices, int pricesSize ) {
    int profit = 0;

    for ( int i = 1; i < pricesSize; i++ ) {
        if ( prices[i - 1] < prices[i] ) {
            profit += prices[i] - prices[i - 1];
        }
    }

    return profit;
}