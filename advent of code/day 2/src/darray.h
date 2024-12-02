#ifndef DARRAY_H
#define DARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef void ( *f_destructor )( void** );
typedef struct dynamicArray dArray_t;

dArray_t* CreateDArray( size_t initial_size, size_t element_size, f_destructor destructor );

void* DArrayAt( dArray_t* darray, size_t index );
bool DArrayPush( dArray_t* darray, void* data );

void DArrayPop( dArray_t* darray );
size_t DArrayUsed( dArray_t* darray );
bool DArrayResize( dArray_t* darray, size_t newSize );

void DestroyDArray( dArray_t** darray );
#endif