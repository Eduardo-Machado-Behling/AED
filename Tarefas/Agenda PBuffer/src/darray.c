#include "darray.h"
#include "arena.h"
#include "common.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t* DArrayGetUsed( void* darray );
static uint32_t* DArrayGetCapacity( void* darray );

void CreateDArray( void** darray ) {
    ARENA_ALLOC( *darray, DARRAY_INITIAL_SIZE );

    if ( !*darray ) {
        return;
    }

    *DArrayGetUsed( *darray )     = DARRAY_HEADER_SIZE;
    *DArrayGetCapacity( *darray ) = DARRAY_INITIAL_SIZE;

    *DArrayGetArg( *darray )      = 0U;
    *DArrayGetElemSize( *darray ) = 0U;
}

uint32_t* DArrayGetArg( void* darray ) {
    return darray;
}
uint32_t* DArrayGetElemSize( void* darray ) {
    return OFFSET_BY( darray, 4U );
}

uint32_t const* DArraySize( void* darray ) {
    return DArrayGetUsed( darray );
}
uint32_t const* DArrayCapacity( void* darray ) {
    return DArrayGetCapacity( darray );
}

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

    void* element = OFFSET_BY( darray, *DArraySize( darray ) );
    *DArrayGetUsed( darray ) += *DArrayGetArg( darray );
    return element;
}

void* DArrayAt( void* darray ) {
    if ( *DArrayGetArg( darray ) < *DArrayGetUsed( darray ) ) {
        return OFFSET_BY( darray, *DArrayGetArg( darray ) );
    }
    return NULL;
}

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

void DArrayPop( void* darray ) {
    *DArrayGetUsed( darray ) -= *DArrayGetArg( darray );
}

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

static uint32_t* DArrayGetUsed( void* darray ) {
    return OFFSET_BY( darray, 8U );
}
static uint32_t* DArrayGetCapacity( void* darray ) {
    return OFFSET_BY( darray, 12U );
}
