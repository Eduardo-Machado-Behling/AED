#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

enum SCAN {
    SCAN_VERTICAL,
    SCAN_HORIZONTAL,
    SCAN_BOX,
};

bool isValidSudoku( char** board, int boardSize, int* boardColSize ) {
    bool array[3][10] = { { 0 }, { 0 } };

    for ( size_t i = 0; i < boardSize; i++ ) {
        memset( array, 0, sizeof( array ) );
        for ( size_t j = 0; j < *boardColSize; j++ ) {
            size_t ivertical   = board[i][j] == '.' ? 10 : board[i][j] - '0';
            size_t ihorizontal = board[j][i] == '.' ? 10 : board[j][i] - '0';

            size_t ybox = ( i * 3 ) % 9 + ( j / 3 );
            size_t xbox = ( i / 3 ) * 3 + ( j % 3 );
            size_t ibox = board[ybox][xbox] == '.' ? 10 : board[ybox][xbox] - '0';

            if ( ivertical != 10 ) {
                if ( array[SCAN_VERTICAL][ivertical] )
                    return false;
                array[SCAN_VERTICAL][ivertical] = true;
            }

            if ( ihorizontal != 10 ) {
                if ( array[SCAN_HORIZONTAL][ihorizontal] )
                    return false;
                array[SCAN_HORIZONTAL][ihorizontal] = true;
            }

            if ( ibox != 10 ) {
                if ( array[SCAN_BOX][ibox] )
                    return false;
                array[SCAN_BOX][ibox] = true;
            }
        }
    }

    return true;
}