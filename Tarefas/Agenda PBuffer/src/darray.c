#include "darray.h"
#include "arena.h"
#include "common.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t* DArrayGetUsed( void* darray );
static uint32_t* DArrayGetCapacity( void* darray );

void* CreateDArray( void ) {
    void* darray = ARENA_ALLOC( DARRAY_INITIAL_SIZE );

    if ( ARENA_INVALID( darray ) ) {
        return ARENA_INVALID_ACCESSOR;
    }

    *DArrayGetUsed( darray )     = DARRAY_HEADER_SIZE;
    *DArrayGetCapacity( darray ) = DARRAY_INITIAL_SIZE;

    *DArrayGetArg( darray )      = 0U;
    *DArrayGetElemSize( darray ) = 0U;

    return darray;
}

/*
=================
DArrayGetArg

return the address of the uint32_t that is used as a argument in the functions:
- DArrayPush/DARRAY_PUSH: Amount of Bytes to push.
- DArrayPop/DARRAY_POP: Amount of Bytes to pop.
- DArrayAt/DARRAY_AT: Amount of Bytes to offset.
- DArrayRemove/DARRAY_REMOVE: Amount of Bytes to offset to get to element to delete.
=================
*/
uint32_t* DArrayGetArg( void* darray ) {
    return ARENA_GET( uint32_t*, darray );
}

/*
=================
DArrayGetElemSize

returns the address of the uint32_t that is used as a argument in the functions:
- DArrayRemove/DARRAY_REMOVE: Size of the element to remove.
=================
*/
uint32_t* DArrayGetElemSize( void* darray ) {
    return OFFSET_BY( ARENA_GETI( darray ), 4U );
}

/*
=================
DArraySize

returns the address of the uint32_t that contains the amount of Bytes used.
=================
*/
uint32_t const* DArraySize( void* darray ) {
    return DArrayGetUsed( darray );
}

/*
=================
DArrayCapacity

returns the address of the uint32_t that contains the amount of Bytes allocated.
=================
*/
uint32_t const* DArrayCapacity( void* darray ) {
    return DArrayGetCapacity( darray );
}

/*
=================
DArrayPush

Allocates *(DArrayGetArg()) Bytes of memory at the end of the DArray and
returns the address of that data to be INITIALIZED and USED,

Required to set *(DArrayGetArg()) first or use the macro DARRAY_PUSHB(DARRAY, BYTES)
or if using in a array fashion DARRAY_PUSH(DARRAY, DATATYPE).
=================
*/
void* DArrayPush( void* darray ) {
    if ( *DArrayGetArg( darray ) >
         *DArrayCapacity( darray ) - *DArrayGetUsed( darray ) ) {
        darray = ARENA_REALLOC(
            darray,
            *DArrayGetCapacity( darray ) +
                RESIZING_FORMULA( *DArrayCapacity( darray ) - *DArrayGetUsed( darray ),
                                  *DArrayGetArg( darray ), DARRAY_GROWTH_AMOUNT ) );

        *DArrayGetCapacity( darray ) +=
            RESIZING_FORMULA( *DArrayCapacity( darray ) - *DArrayGetUsed( darray ),
                              *DArrayGetArg( darray ), DARRAY_GROWTH_AMOUNT );
    }

    void* element = OFFSET_BY( ARENA_GETI( darray ), *DArraySize( darray ) );
    *DArrayGetUsed( darray ) += *DArrayGetArg( darray );
    return element;
}

/*
=================
DArrayAt

Returns the address of the data in the offset *(DArrayGetArg()) Bytes,
Required to set *(DArrayGetArg()) first or use the macro DARRAY_ATB(DARRAY, OFFSET)
or if using in a array fashion DARRAY_AT(DARRAY, INDEX, DATATYPE).
=================
*/
void* DArrayAt( void* darray ) {
    if ( *DArrayGetArg( darray ) < *DArrayGetUsed( darray ) ) {
        return OFFSET_BY( ARENA_GETI( darray ), *DArrayGetArg( darray ) );
    }
    return NULL;
}

/*
=================
DArrayRemove

Removes the element at the offset *(DArrayGetArg()) Bytes and with size *(DArrayGetElemSize()),
Required to set *(DArrayGetArg()) and *(DArrayGetElemSize()) first,
or use the macro DARRAY_REMOVEB(DARRAY, OFFSET, BYTES),
or if using in a array fashion DARRAY_PUSH(DARRAY, INDEX, DATATYPE).
=================
*/
void DArrayRemove( void* darray ) {
    if ( *DArraySize( darray ) - *DArrayGetArg( darray ) >
         *DArrayGetElemSize( darray ) ) {
        void* dest = DArrayAt( darray );
        void* src =
            ( *DArrayGetArg( darray ) += *DArrayGetElemSize( darray ), DArrayAt( darray ) );
        memmove( dest, src, *DArrayGetUsed( darray ) - *DArrayGetArg( darray ) );
    }
    *DArrayGetArg( darray ) = *DArrayGetElemSize( darray );
    DArrayPop( darray );
}

/*
=================
DArrayPop


Removes the last (rightmost) *(DArrayGetArg()) Bytes of memory,
Required to set *(DArrayGetArg()) first or use the macro DARRAY_POPB(DARRAY, BYTES)
or if using in a array fashion DARRAY_POP(DARRAY, DATATYPE).
=================
*/
void DArrayPop( void* darray ) {
    *DArrayGetUsed( darray ) -= *DArrayGetArg( darray );
}

/*
=================
DestroyDArray

Frees a instance of DArray
=================
*/
void DestroyDArray( void** darray ) {
    if ( !darray ) {
        return;
    }

    void* da = *darray;
    *darray  = NULL;
    if ( da ) {
        ARENA_FREE( da );
    }
}

/*
=================
DArrayGetUsed

Returns the address of the uint32_t that contains the amount of Bytes used.
=================
*/
static uint32_t* DArrayGetUsed( void* darray ) {
    return OFFSET_BY( ARENA_GETI( darray ), 8U );
}

/*
=================
DArrayGetCapacity

Returns the address of the uint32_t that contains the amount of Bytes allocated.
=================
*/
static uint32_t* DArrayGetCapacity( void* darray ) {
    return OFFSET_BY( ARENA_GETI( darray ), 12U );
}
