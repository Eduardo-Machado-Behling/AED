#ifndef DARRAY_H
#define DARRAY_H

#include <stdint.h>

#define DARRAY_INITIAL_SIZE 20U
#define DARRAY_GROWTH_AMOUNT 8U

#define DARRAY_HEADER_SIZE 16U
/*
 * DArray Layout:
 * Offset (B) |     Type    | Content             | Reason            |
 *      0     | uint32_t    | Multi Purpose Arg   |  Function Args    |
 *      4     | uint32_t    | Element Size Arg    |  Remove Function  |
 *      8     | uint32_t    | Buffer Used (B)     |  Dynamic Resizing |
 *     12     | uint32_t    | Buffer Capacity (B) |  Dynamic Resizing |
 *     16     | void*       | Data Array          |  Data storage     |
 *     ...
 */

#define DARRAY_ELEMENT_AMOUNT( DARRAY, DATATYPE ) \
    ( ( *DArraySize( DARRAY ) - DARRAY_HEADER_SIZE ) / sizeof( DATATYPE ) )
#define DARRAY_AT( DARRAY, INDEX, DATATYPE )                                     \
    ( (DATATYPE*) ( *DArrayGetArg( DARRAY ) =                                    \
                        ( ( INDEX ) * sizeof( DATATYPE ) ) + DARRAY_HEADER_SIZE, \
                    DArrayAt( DARRAY ) ) )

#define DARRAY_REMOVE( DARRAY, INDEX, DATATYPE )                                              \
    ( *DArrayGetArg( DARRAY )      = ( ( INDEX ) * sizeof( DATATYPE ) ) + DARRAY_HEADER_SIZE, \
      *DArrayGetElemSize( DARRAY ) = sizeof( DATATYPE ), DArrayRemove( DARRAY ) )

#define DARRAY_PUSH( DARRAY, DATATYPE ) \
    ( *DArrayGetArg( DARRAY ) = sizeof( DATATYPE ), DArrayPush( DARRAY ) )

#define DARRAY_POP( DARRAY, DATATYPE ) \
    ( *DArrayGetArg( DARRAY ) = sizeof( DATATYPE ), DArrayPop( DARRAY ) )

void CreateDArray( void** darray );

uint32_t* DArrayGetArg( void* darray );
uint32_t* DArrayGetElemSize( void* darray );

uint32_t const* DArraySize( void* darray );
uint32_t const* DArrayCapacity( void* darray );

void* DArrayPush( void* darray );
void* DArrayAt( void* darray );
void DArrayRemove( void* darray );
void DArrayPop( void* darray );

void DestroyDArray( void** darray );

#endif
