#define MAX_CLASSES 100000

#define GET_PARENT( i ) ( ( i >> 1 ) - 1 )
#define GET_LEFT( i ) ( ( i << 1 ) + 1 )  // 2i + 1
#define GET_RIGHT( i ) ( ( i + 1 ) << 1 ) // 2i + 2 == 2(i + 1)

struct entry {
    int* ref;
    double delta;
};

static void swap( struct entry* a, struct entry* b ) {
    struct entry temp = *a;
    *a                = *b;
    *b                = temp;
}

static void heapify( struct entry heap[MAX_CLASSES], int used, int i ) {
    int largest = i;
    int left    = GET_LEFT( i );
    int right   = GET_RIGHT( i );

    if ( left < used && ( heap[left].delta > heap[largest].delta ) )
        largest = left;

    if ( right < used && ( heap[right].delta > heap[largest].delta ) )
        largest = right;

    if ( largest != i ) {
        swap( &heap[i], &heap[largest] );
        heapify( heap, used, largest );
    }
}

static void heapSort( struct entry heap[MAX_CLASSES], int used ) {
    for ( int i = GET_PARENT( used ); i >= 0; i-- ) {
        heapify( heap, used, i );
    }
}

double heapRMaxAverageRatio( int** classes, int classesSize, int* classesColSize,
                             int extraStudents ) {
    struct entry heap[MAX_CLASSES] = { 0 };
    int used                       = 0;
    double res                     = 0;

    for ( int i = 0; i < classesSize; i++ ) {
        double change = (double) ( classes[i][0] ) / ( classes[i][1] );
        res += change;
        heap[used].ref     = classes[i];
        heap[used++].delta = (double) ( classes[i][0] + 1 ) / ( classes[i][1] + 1 ) - change;
    }

    heapSort( heap, used );

    for ( int i = 0; i < extraStudents; i++ ) {
        double max = heap[0].delta;
        res += max;
        int* ref      = heap[0].ref;
        double change = (double) ++ref[0] / ++ref[1];
        heap[0].delta = ( (double) ( ref[0] + 1 ) / ( ref[1] + 1 ) ) - change;

        heapify( heap, used, 0 );
    }

    return res / classesSize;
}
