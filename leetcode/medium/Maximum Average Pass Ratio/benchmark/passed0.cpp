#include <stdio.h>
#include <stdlib.h>

/*
===============================================================================

Heap Implementation for Maximizing Average Ratio
Author: Andressa Von Ahnt

===============================================================================
*/

typedef struct node_t {
    int pass;
    int total;
    double difference;
} node_t;

typedef struct heap_t {
    node_t* heapArray;
    double average;
    int size;
    int capacity;
} heap_t;

/*
====================
PushNode

Inserts a new node into the heap and maintains the max heap property.
====================
*/
void PushNode( heap_t* heap, int pass, int total ) {
    if ( heap->size == heap->capacity ) {
        printf( "Empty heap!\n" );
        return;
    }

    double difference = ( ( (double) pass + 1 ) / ( (double) total + 1 ) ) - ( (double) pass / (double) total );

    node_t newNode;
    newNode.pass       = pass;
    newNode.total      = total;
    newNode.difference = difference;

    int i              = heap->size;
    heap->heapArray[i] = newNode;

    while ( i > 0 && heap->heapArray[( i - 1 ) / 2].difference < heap->heapArray[i].difference ) {
        node_t temp                    = heap->heapArray[i];
        heap->heapArray[i]             = heap->heapArray[( i - 1 ) / 2];
        heap->heapArray[( i - 1 ) / 2] = temp;
        i                              = ( i - 1 ) / 2;
    }

    heap->average += (double) pass / (double) total;
    heap->size++;
}

/*
====================
PopNode

Removes and returns the node removed that has the highest difference from the heap.
====================
*/
node_t PopNode( heap_t* heap ) {
    if ( heap->size == 0 ) {
        printf( "Heap vazia!\n" );
        return ( node_t ){ 0, 0, 0.0 };
    }

    node_t root = heap->heapArray[0];

    heap->average -= ( (double) ( heap->heapArray[0].pass ) / (double) ( heap->heapArray[0].total ) );
    heap->heapArray[0] = heap->heapArray[heap->size - 1];
    heap->size--;

    int i = 0;
    while ( 2 * i + 1 < heap->size ) {
        int biggerSon = 2 * i + 1;
        if ( biggerSon + 1 < heap->size && heap->heapArray[biggerSon].difference < heap->heapArray[biggerSon + 1].difference ) {
            biggerSon++;
        }

        if ( heap->heapArray[i].difference >= heap->heapArray[biggerSon].difference ) {
            break;
        }

        node_t aux                 = heap->heapArray[i];
        heap->heapArray[i]         = heap->heapArray[biggerSon];
        heap->heapArray[biggerSon] = aux;
        i                          = biggerSon;
    }

    return root;
}

/*
====================
MaxAverageRatio

Computes the maximum average ratio by distributing extra students optimally.
====================
*/
double passed0MaxAverageRatio( int** classes, int classesSize, int* classesColSize, int extraStudents ) {
    heap_t* heap = (heap_t*) malloc( sizeof( heap_t ) );
    if ( heap == NULL ) {
        return 0;
    }

    heap->heapArray = (node_t*) malloc( classesSize * sizeof( node_t ) );
    if ( heap->heapArray == NULL ) {
        free( heap );
        return 0;
    }

    heap->size     = 0;
    heap->average  = 0;
    heap->capacity = classesSize;

    for ( int i = 0; i < classesSize; i++ ) {
        PushNode( heap, classes[i][0], classes[i][1] );
    }

    for ( int i = 0; i < extraStudents; i++ ) {
        node_t top = PopNode( heap );
        top.pass++;
        top.total++;
        PushNode( heap, top.pass, top.total );
    }

    double result = (double) ( heap->average ) / (double) ( heap->size );

    free( heap->heapArray );
    free( heap );

    return result;
}