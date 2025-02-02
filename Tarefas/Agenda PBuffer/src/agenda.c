#include "agenda.h"
#include "arena.h"
#include "common.h"
#include "darray.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void AgendaDynamicCopy( void* agenda, const void* src,
                               const uint32_t* bytesAmount );

/*
==================
AgendaGetData

returns the address of the Data DArray
==================
*/
static void** AgendaGetData( void* agenda ) {
    return (void**) ARENA_GETI( agenda );
}

/*
==================
AgendaGetPrefix

returns the address of the Prefixes DArray
==================
*/
static void** AgendaGetPrefix( void* agenda ) {
    return OFFSET_BY_TYPED( void**, ARENA_GETI( agenda ), 8U );
}

/*
==================
AgendaGetUsed

returns the address of the uint32_t that has how many Bytes are used.
==================
*/
static uint32_t const* AgendaGetUsed( void* agenda ) {
    return DArraySize( *AgendaGetData( agenda ) );
}

/*
==================
CreateAgenda

Creates a instance of Agenda to use with the functions named Agenda* or macros named AGENDA_*
returns ARENA_INVALID_ACCESSOR on failure, failure points are ARENA_ALLOC and CreateDArray()
==================
*/
void* CreateAgenda( void ) {
    void* agenda = ARENA_ALLOC( AGENDA_SIZE );
    if ( ARENA_INVALID( agenda ) ) {
        return ARENA_INVALID_ACCESSOR;
    }

    // Need a aux variable, because the compiler
    // wasn't working with:
    // *AgendaGetData(agenda) = CreateDArray();
    //                          _       _
    //  For some magical reason  \(-_-)/
    void* aux                = CreateDArray();
    *AgendaGetData( agenda ) = aux;

    aux                        = CreateDArray();
    *AgendaGetPrefix( agenda ) = aux;

    *AgendaGetIndexArg( agenda )  = 0U;
    *AgendaGetSearchArg( agenda ) = NULL;

    return agenda;
}

/*
==================
AgendaGetIndexArg

returns the address of the uint32_t that is used on the following function as a argument
- AgendaGetEntry/AGENDA_GET_ENTRY: the index, i.e. 0 -> entry #1, 4 -> entry #5, ...
==================
*/
uint32_t* AgendaGetIndexArg( void* agenda ) {
    return DArrayGetArg( *AgendaGetData( agenda ) );
}

/*
==================
AgendaGetSearchArg

return the address of the const char** that is used to provide the argument for the functions:abort
- AgendaRemove/AGENDA_REMOVE: name of the entry to remove.
- AgendaSearch/AGENDA_SEARCH: name of the entry to find.
==================
*/
const char** AgendaGetSearchArg( void* agenda ) {
    return OFFSET_BY_TYPED( const char**, ARENA_GETI( agenda ), 16U );
}

/*
==================
AgendaGetEntry

Return the void* of the entry at *(AgendaGetEntry) index,
need to set it before using or use the macro ARENA_GET_ENTRY(AGENDA, INDEX) directly.

Returns NULL on invalid index.
==================
*/
void* AgendaGetEntry( void* agenda ) {
    uint32_t* index =
        DARRAY_AT( *AgendaGetPrefix( agenda ), *AgendaGetIndexArg( agenda ), uint32_t );

    if ( !index ) {
        return NULL;
    }

    *DArrayGetArg( *AgendaGetData( agenda ) ) = *index;

    return DArrayAt( *AgendaGetData( agenda ) );
}

/*
==================
AgendaSearch

Returns the void* of the entry with the name *(AgendaGetSearchArg),
need to be set before using or use the macro AGENDA_SEARCH(AGENDA, NAME) directly.

Returns NULL on name not found.
==================
*/
void* AgendaSearch( void* agenda ) {
    uint32_t* i = ARENA_ALLOC( sizeof( *i ) );

    void* entry = NULL;
    for ( *ARENA_GETI( i ) = 0; 1; *ARENA_GETI( i ) += 1 ) {
        entry = AGENDA_GET_ENTRY( agenda, *ARENA_GETI( i ) );
        if ( !entry ) {
            break;
        }

        if ( strcmp( entry, ARENA_GET( const char*, *AgendaGetSearchArg( agenda ) ) ) ==
             0 ) {
            break;
        }
    }

    ARENA_FREE( i );
    return entry;
}

/*
==================
AgendaAdd

Adds a entry to the Agenda.
name, age, email needs to be ARENA_ALLOCATED.
==================
*/
void AgendaAdd( void* agenda, const char* name, uint8_t* age,
                const char* email ) {
    ARENA_SIH( uint32_t, i );
    ARENA_SIH( uint32_t, used );
    ARENA_SIH( uint32_t, bytesAdded );

    void** prefix = AgendaGetPrefix( agenda );
    void** data   = AgendaGetData( agenda );

    *ARENA_GETI( bytesAdded ) = 0;
    *ARENA_GETI( i )          = strlen( ARENA_GETI( name ) ) + 1;
    *ARENA_GETI( used )       = *AgendaGetUsed( agenda );

    uint32_t* temp = DARRAY_PUSH( *prefix, uint32_t );
    *temp          = *ARENA_GETI( used );

    AgendaDynamicCopy( agenda, name, i );
    // copy failed
    if ( *ARENA_GETI( used ) == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }

    *ARENA_GETI( bytesAdded ) += *ARENA_GETI( i );

    *ARENA_GETI( i )    = sizeof( *age );
    *ARENA_GETI( used ) = *AgendaGetUsed( agenda );
    AgendaDynamicCopy( agenda, age, i );
    // copy failed
    if ( *ARENA_GETI( used ) == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }
    *ARENA_GETI( bytesAdded ) += *ARENA_GETI( i );

    *ARENA_GETI( i )    = strlen( ARENA_GETI( email ) ) + 1;
    *ARENA_GETI( used ) = *AgendaGetUsed( agenda );
    AgendaDynamicCopy( agenda, email, i );
    // copy failed
    if ( *ARENA_GETI( used ) == *AgendaGetUsed( agenda ) ) {
        goto copy_failed;
    }

    return;

copy_failed:
    *DArrayGetArg( *data ) = *ARENA_GETI( bytesAdded );
    DArrayPop( *data );
    DARRAY_POP( *prefix, uint32_t );
}

/*
=================
AgendaRemove

Removes the entry with the name *(AgendaGetSearchArg),
need to be set before using or use the macro AGENDA_REMOVE(AGENDA, NAME) directly.

On name not found returns NULL.
On removed successfully return the old entry address,
this void* should not be used, just test on conditions.
=================
*/
void* AgendaRemove( void* agenda ) {
    ARENA_SIH( uint32_t, i );
    ARENA_SIH( uint32_t, elemAmount );

    *ARENA_GETI( elemAmount ) =
        DARRAY_ELEMENT_AMOUNT( *AgendaGetPrefix( agenda ), uint32_t );
    for ( *ARENA_GETI( i ) = 0; *ARENA_GETI( i ) < *ARENA_GETI( elemAmount );
          *ARENA_GETI( i ) += 1 ) {
        void* entry = AGENDA_GET_ENTRY( agenda, *ARENA_GETI( i ) );
        if ( !entry ) {
            break;
        }

        if ( strcmp( entry, ARENA_GET( const char*, *AgendaGetSearchArg( agenda ) ) ) ==
             0 ) {
            if ( *ARENA_GETI( i ) + 1 == *ARENA_GETI( elemAmount ) ) {
                *DArrayGetArg( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ), uint32_t ) -
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ) - 1, uint32_t );

                DArrayPop( *AgendaGetData( agenda ) );
                DARRAY_POP( *AgendaGetPrefix( agenda ), uint32_t );
            } else {
                *DArrayGetArg( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ), uint32_t );

                *DArrayGetElemSize( *AgendaGetData( agenda ) ) =
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ) + 1, uint32_t ) -
                    *DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ), uint32_t );

                DARRAY_REMOVE( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ), uint32_t );
                DArrayRemove( *AgendaGetData( agenda ) );

                for ( ; 1; *ARENA_GETI( i ) += 1 ) {
                    uint32_t* index =
                        DARRAY_AT( *AgendaGetPrefix( agenda ), *ARENA_GETI( i ), uint32_t );
                    if ( !index ) {
                        break;
                    }
                    *index -= *DArrayGetElemSize( *AgendaGetData( agenda ) );
                }
            }
            return entry;
        }
    }

    return NULL;
}

/*
=================
DestroyAgenda

Free the Agenda instance.
=================
*/
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

/*
==================
AgendaDynamicCopy

Copies data from src into de Data Darray, using its API.
==================
*/
static void AgendaDynamicCopy( void* agenda, const void* src,
                               const uint32_t* bytesAmount ) {
    void* data = *AgendaGetData( agenda );

    *DArrayGetArg( data ) = *ARENA_GETI( bytesAmount );
    void* dest            = DArrayPush( data );
    memmove( dest, ARENA_GETI( src ), *ARENA_GETI( bytesAmount ) );
}
