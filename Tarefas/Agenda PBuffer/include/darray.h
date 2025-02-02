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

#define DARRAY_ATB( DARRAY, OFFSET )                             \
    ( *DArrayGetArg( DARRAY ) = ( OFFSET ) + DARRAY_HEADER_SIZE, \
      DArrayAt( DARRAY ) )

#define DARRAY_AT( DARRAY, INDEX, DATATYPE ) \
    ( (DATATYPE*) ( DARRAY_ATB( DARRAY, ( INDEX ) * sizeof( DATATYPE ) ) ) )

#define DARRAY_REMOVEB( DARRAY, OFFSET, BYTES )                       \
    ( *DArrayGetArg( DARRAY )      = ( OFFSET ) + DARRAY_HEADER_SIZE, \
      *DArrayGetElemSize( DARRAY ) = BYTES, DArrayRemove( DARRAY ) )

#define DARRAY_REMOVE( DARRAY, INDEX, DATATYPE ) \
    DARRAY_REMOVEB( DARRAY, INDEX * sizeof( DATATYPE ), sizeof( DATATYPE ) )

#define DARRAY_PUSHB( DARRAY, BYTES ) \
    ( *DArrayGetArg( DARRAY ) = BYTES, DArrayPush( DARRAY ) )
#define DARRAY_PUSH( DARRAY, DATATYPE ) \
    DARRAY_PUSHB( DARRAY, sizeof( DATATYPE ) )

#define DARRAY_POPB( DARRAY, BYTES ) \
    ( *DArrayGetArg( DARRAY ) = BYTES, DArrayPop( DARRAY ) )
#define DARRAY_POP( DARRAY, DATATYPE ) \
    DARRAY_POPB( DARRAY, sizeof( DATATYPE ) )

void* CreateDArray( void );

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
