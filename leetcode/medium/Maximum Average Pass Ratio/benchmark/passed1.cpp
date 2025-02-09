#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
    int array[2];
    double dif;
};

static double maxAverageRatio( int** classees, int classeesSize, int* classeesColSize, int extraStudents );
static void Push( int* classe, Node* heap, int* size );
static void Heapfy( Node* heap, int size, int pos );
static void AddStudents( int extraStudents, Node* heap, int* size );

static void Heapfy( Node* heap, int size, int pos ) {
    int left = 2 * pos + 1, right = 2 * pos + 2, large = pos;
    Node temp;
    if ( left < size && heap[left].dif >= heap[pos].dif ) {
        large = left;
    }

    if ( right < size && heap[right].dif >= heap[pos].dif && heap[right].dif >= heap[left].dif ) {
        large = right;
    }
    if ( large != pos ) {
        temp        = heap[large];
        heap[large] = heap[pos];
        heap[pos]   = temp;
        Heapfy( heap, size, large );
    }
}

static void Push( int* classe, Node* heap, int* size ) {
    Node novo;
    novo.array[0] = classe[0];
    novo.array[1] = classe[1];
    novo.dif      = (double) ( (double) ( novo.array[0] + 1 ) / (double) ( novo.array[1] + 1 ) - (double) ( novo.array[0] ) / (double) ( novo.array[1] ) );

    *size += 1;
    heap[*size - 1] = novo;
}

static void AddStudents( int students, Node* heap, int* size ) {
    int classe[2];
    while ( students > 0 ) {
        heap[0].array[0] += 1;
        heap[0].array[1] += 1;
        students -= 1;
        heap[0].dif = (double) ( (double) ( heap[0].array[0] + 1 ) / (double) ( heap[0].array[1] + 1 ) - (double) ( heap[0].array[0] ) / (double) ( heap[0].array[1] ) );
        classe[0]   = heap[0].array[0];
        classe[1]   = heap[0].array[1];
        heap[0]     = heap[*size - 1];
        *size -= 1;
        Push( classe, heap, size );
        for ( int i = ( *size - 2 ) / 2; i > 0; i = ( i - 1 ) / 2 ) {
            Heapfy( heap, *size, i );
        }
        Heapfy( heap, *size, 0 );
    }
}

double passed1MaxAverageRatio( int** classees, int classeesSize, int* classeesColSize, int extraStudents ) {
    Node* heap = (Node*) malloc( sizeof( Node ) * ( 5 + classeesSize ) );
    int size   = 0;
    double ans = 0.0;
    for ( int index = 0; index < classeesSize; index++ ) {
        Push( &classees[index][0], heap, &size );
    }
    for ( int i = size / 2 - 1; i >= 0; i-- ) {
        Heapfy( heap, size, i );
    }

    AddStudents( extraStudents, heap, &size );

    for ( int index = 0; index < classeesSize; index++ ) {
        ans += (double) ( heap[index].array[0] ) / (double) ( heap[index].array[1] );
    }

    return ans / (double) ( classeesSize );
}