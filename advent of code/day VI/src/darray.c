#include "darray.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MIN( X, Y ) ( ( X ) < ( Y ) ? ( X ) : ( Y ) )

typedef struct dynamicArray {
    uint8_t* data;
    size_t used;
    size_t size;
    size_t elem_size;

    f_destructor destructor;
} dArray_t;

dArray_t* CreateDArray( size_t initial_size, size_t element_size, f_destructor destructor ) {
    dArray_t* darray = malloc( sizeof( dArray_t ) );
    if ( !darray )
        return NULL;

    darray->used       = 0;
    darray->size       = initial_size;
    darray->elem_size  = element_size;
    darray->destructor = destructor;
    darray->data       = malloc( darray->size * darray->elem_size );
    if ( !darray->data )
        DestroyDArray( &darray );

    return darray;
}

void* DArrayAt( dArray_t* darray, size_t index ) {
    if ( index >= darray->used )
        return NULL;

    return darray->data + index * darray->elem_size;
}

bool DArrayPush( dArray_t* darray, void* data ) {
    if ( darray->used == darray->size ) {
        if ( !DArrayResize( darray, darray->size + MIN( darray->size * 2, 200 ) ) )
            return false;
    }

    memcpy( darray->data + ( darray->used++ * darray->elem_size ), data, darray->elem_size );

    return true;
}

bool DArrayAssign( dArray_t* darray, size_t i, void* data ) {
    memcpy( darray->data + ( i * darray->elem_size ), data, darray->elem_size );

    return true;
}

bool DArrayResize( dArray_t* darray, size_t newSize ) {
    void* newMem = realloc( darray->data, newSize * darray->elem_size );
    if ( !newMem )
        return false;

    darray->size = newSize;
    darray->data = newMem;

    return true;
}

size_t DArrayUsed( dArray_t* darray ) {
    return darray->used;
}

void DArrayClear( dArray_t* darray ) {
    darray->used = 0;
}

void DArrayPop( dArray_t* darray ) {
    darray->used--;
}

void DestroyDArray( dArray_t** darray ) {
    if ( !darray )
        return;

    dArray_t* da = *darray;
    *darray      = NULL;

    if ( !da )
        return;

    if ( da->data ) {
        if ( da->destructor ) {
            for ( size_t i = 0; i < da->used; i++ )
                da->destructor( DArrayAt( da, i ) );
        }
        free( da->data );
    }

    free( da );
}
