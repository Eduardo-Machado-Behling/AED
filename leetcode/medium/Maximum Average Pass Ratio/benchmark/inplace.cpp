
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double getPotentialRatio( int* a ) {
    return (double) ( a[0] + 1 ) / ( a[1] + 1 ) - ( (double) a[0] / a[1] );
}

int compare( const void* pa, const void* pb ) {
    int* a = *(int**) pa;
    int* b = *(int**) pb;

    return getPotentialRatio( a ) > getPotentialRatio( b );
}

double inplaceMaxAverageRatio( int** classes, int classesSize, int* classesColSize,
                               int extraStudents ) {

    qsort( (void*) classes, classesSize, sizeof( int* ), compare );

    double ratio = 0;
    int a[2]     = { 0 };
    for ( size_t i = 0; i < extraStudents; i++ ) {
        int* temp = classes[classesSize - 1];
        a[0]      = temp[0] + 1;
        a[1]      = temp[1] + 1;
        ratio     = getPotentialRatio( a );

        int j = classesSize - 2;
        for ( ; j >= 0 && ratio < getPotentialRatio( classes[j] ); i-- ) {
            classes[j + 1][0] = classes[j][0];
            classes[j + 1][1] = classes[j][1];
        }
        classes[j + 1][0] = a[0];
        classes[j + 1][1] = a[1];
    }

    double res = 0;
    for ( size_t i = 0; i < classesSize; i++ ) {
        res += (double) classes[i][0] / classes[i][1];
    }

    return res / classesSize;
}
