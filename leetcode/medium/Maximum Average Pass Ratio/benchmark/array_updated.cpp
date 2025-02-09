#include <cstdint>
#include <cstdlib>

#define MAX_CLASSES 100000

struct entry {
    int students;
    int passed;
    double change;
} __attribute__( ( aligned( 16 ) ) );

static void appendMin( struct entry minStack[MAX_CLASSES], int* used,
                       struct entry entry ) {
    int i = ( *used )++ - 1;
    for ( ; i >= 0 && minStack[i].change > entry.change; i-- ) {
        minStack[i + 1] = minStack[i];
    }

    minStack[i + 1] = entry;
}

static double getNextChange( int passed, int students ) {
    return ( (double) ( passed + 1 ) / ( students + 1 ) );
}

static double getChange( int passed, int students ) {
    return (double) ( students - passed ) / ( students * ( students + 1.0 ) );
}

double arrayUpdatedMaxAverageRatio( int** classes, int classesSize, int* classesColSize,
                                    int extraStudents ) {
    struct entry minStack[MAX_CLASSES] = { 0 };

    int used   = 0;
    double res = 0;

    for ( int i = 0; i < classesSize; i++ ) {
        double change = (double) ( classes[i][0] ) / ( classes[i][1] );
        res += change;
        appendMin( minStack, &used,
                   ( struct entry ){ .students = classes[i][1],
                                     .passed   = classes[i][0],
                                     .change   = getNextChange( classes[i][0], classes[i][1] ) - change } );
    }

    struct entry* maxChange = minStack + used - 1;
    for ( int i = 0; i < extraStudents; i++ ) {
        res += maxChange->change;
        maxChange->change = getChange( ++maxChange->passed, ++maxChange->students );
        --used;
        appendMin( minStack, &used, *maxChange );
    }

    return res / used;
}
