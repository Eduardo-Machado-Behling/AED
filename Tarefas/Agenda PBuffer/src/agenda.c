#include "agenda.h"
#include "arena.h"
#include "common.h"
#include "darray.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void** AgendaGetData( void* agenda ) {
    return (void**) agenda;
}

static void** AgendaGetPrefix( void* agenda ) {
    return OFFSET_BY_TYPED( void**, agenda, 8U );
}

static uint32_t const* AgendaGetUsed( void* agenda ) {
    return DArraySize( *AgendaGetData( agenda ) );
}

static void AgendaDynamicCopy( void* agenda, const void* src,
                               const uint32_t* bytesAmount );

void CreateAgenda( void** agenda ) {
    ARENA_ALLOC( *agenda, AGENDA_SIZE );
    if ( !*agenda ) {
        return;
    }

    // Need a aux variable, because the compiler
    // wasn't working with:
    // *AgendaGetData(agenda) = CreateDArray();
    //                          _       _
    //  For some magical reason  \(-_-)/
    CreateDArray( AgendaGetData( *agenda ) );
    CreateDArray( AgendaGetPrefix( *agenda ) );

    *AgendaGetIndexArg( *agenda )  = 0U;
    *AgendaGetSearchArg( *agenda ) = NULL;
}

uint32_t* AgendaGetIndexArg( void* agenda ) {
    return DArrayGetArg( *AgendaGetData( agenda ) );
}

const char** AgendaGetSearchArg( void* agenda ) {
    return OFFSET_BY_TYPED( const char**, agenda, 16U );
}

// Need to use "AgendaSetGetArg" to specify index,
// returns NULL on invalid index.
void* AgendaGetEntry( void* agenda ) {
    uint32_t* index =
        DARRAY_AT( *AgendaGetPrefix( agenda ), *AgendaGetIndexArg( agenda ), uint32_t );

    if ( !index ) {
        return NULL;
    }

    *DArrayGetArg( *AgendaGetData( agenda ) ) = *index;

    return DArrayAt( *AgendaGetData( agenda ) );
}

// Need to use "AgendaGetSearchArg" to specify the name to search,
// returns NULL on not found.
void* AgendaSearch( void* agenda ) {
    ARENA_SIH( uint32_t, i );

    void* entry = NULL;
    for ( *i = 0; 1; *i += 1 ) {
        entry = AGENDA_GET_ENTRY( agenda, *i );
        if ( !entry ) {
            break;
        }

        if ( strcmp( entry, *AgendaGetSearchArg( agenda ) ) ==
             0 ) {
            break;
        }
    }

    return entry;
}

void AgendaAdd( void* agenda, const char* name, uint8_t* age,
                const char* email ) {
    ARENA_SIH( uint32_t, i );
    ARENA_SIH( uint32_t, used );
    ARENA_SIH( uint32_t, bytesAdded );

    void** prefix = AgendaGetPrefix( agenda );
    void** data   = AgendaGetData( agenda );

    *bytesAdded = 0;
    *i          = strlen( name ) + 1;
    *used       = *AgendaGetUsed( agenda );

    uint32_t* temp = DARRAY_PUSH( *prefix, uint32_t );
    *temp          = *used;

    AgendaDynamicCopy( agenda, name, i );
    // copy failed
    if ( *used == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }

    *bytesAdded += *i;

    *i    = sizeof( *age );
    *used = *AgendaGetUsed( agenda );
    AgendaDynamicCopy( agenda, age, i );
    // copy failed
    if ( *used == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }
    *bytesAdded += *i;

    *i    = strlen( email ) + 1;
    *used = *AgendaGetUsed( agenda );
    AgendaDynamicCopy( agenda, email, i );
    // copy failed
    if ( *used == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }

    return;

copy_failed:
    *DArrayGetArg( *data ) = *bytesAdded;
    DArrayPop( *data );
    DARRAY_POP( *prefix, uint32_t );
}

// TODO(yadard): Crazy
void* AgendaRemove( void* agenda ) {
    ARENA_SIH( uint32_t, i );
    ARENA_SIH( uint32_t, elemAmount );

    *elemAmount =
        DARRAY_ELEMENT_AMOUNT( *AgendaGetPrefix( agenda ), uint32_t );
    for ( *i = 0; *i < *elemAmount;
          *i += 1 ) {
        void* entry = AGENDA_GET_ENTRY( agenda, *i );
        if ( !entry ) {
            break;
        }

        if ( strcmp( entry, *AgendaGetSearchArg( agenda ) ) ==
             0 ) {
            if ( *i + 1 == *elemAmount ) {
                *DArrayGetArg( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *i, uint32_t ) -
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *i - 1, uint32_t );

                DArrayPop( *AgendaGetData( agenda ) );
                DARRAY_POP( *AgendaGetPrefix( agenda ), uint32_t );
            } else {
                *DArrayGetArg( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *i, uint32_t );

                *DArrayGetElemSize( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *i + 1, uint32_t ) -
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *i, uint32_t );

                DARRAY_REMOVE( *AgendaGetPrefix( agenda ), *i, uint32_t );
                DArrayRemove( *AgendaGetData( agenda ) );

                for ( ; 1; *i += 1 ) {
                    uint32_t* index =
                        DARRAY_AT( *AgendaGetPrefix( agenda ), *i, uint32_t );
                    if ( !index ) {
                        break;
                    }
                    *index -= *DArrayGetElemSize( *AgendaGetData( agenda ) );
                }
            }
            return (void*) 1;
        }
    }

    return NULL;
}

void DestroyAgenda( void** agenda ) {
    if ( !agenda ) {
        return;
    }

    void* ag = *agenda;
    *agenda  = NULL;

    if ( ag ) {
        if ( *AgendaGetData( ag ) ) {
            DestroyDArray( AgendaGetData( ag ) );
        }
        if ( *AgendaGetPrefix( ag ) ) {
            DestroyDArray( AgendaGetPrefix( ag ) );
        }
        ARENA_FREE( ag );
    }
}

static void AgendaDynamicCopy( void* agenda, const void* src,
                               const uint32_t* bytesAmount ) {
    void* data = *AgendaGetData( agenda );

    *DArrayGetArg( data ) = *bytesAmount;
    void* dest            = DArrayPush( data );
    memmove( dest, src, *bytesAmount );
}
