#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Found a better solution, with a excellent explanation
// a huge thanks to @Himanshu Malik
int maxProfit( const int* prices, int pricesSize ) {
    int minPrice  = *prices;
    int maxProfit = 0;

    for ( int i = 1; i < pricesSize; i++ ) {
        if ( prices[i] < minPrice ) {
            minPrice = prices[i];
        }

        int profit = prices[i] - minPrice;
        if ( maxProfit < profit ) {
            maxProfit = profit;
        }
    }

    return maxProfit;
}