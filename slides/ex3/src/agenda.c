#include "agenda.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef uint8_t byte_t;

#define AGENDA_GET_USED( agenda ) ( *( (size_t*) agenda->pBuffer ) )
#define AGENDA_GET_USED_BYTES( agenda ) ( AGENDA_GET_USED( agenda ) * agenda->size.element )
#define AGENDA_GET_DATA( agenda ) ( ( (byte_t*) agenda->pBuffer ) + sizeof( size_t ) )

#define MIN( a, b ) ( a < b ? a : b )

typedef struct agenda_t {
    void* pBuffer;
    struct {
        size_t buffer;
        size_t element;
    } size;
} Agenda_t;

Agenda_t* create_agenda( size_t initial_size, size_t elem_size ) {
    Agenda_t* agenda = malloc( sizeof( Agenda_t ) );

    agenda->size.buffer  = initial_size;
    agenda->size.element = elem_size;

    agenda->pBuffer = malloc( agenda->size.buffer + sizeof( size_t ) );
    if ( !agenda->pBuffer )
        destroy_agenda( &agenda );

    AGENDA_GET_USED( agenda ) = 0;
    return agenda;
}

void agenda_add( Agenda_t* agenda, void* entry ) {
    size_t availableSize = agenda->size.buffer - AGENDA_GET_USED( agenda ) - sizeof( size_t );

    if ( availableSize < agenda->size.element ) {
        size_t newSize = agenda->size.buffer + MIN( agenda->size.buffer * 2, 255 );
        void* newMem   = realloc( agenda->pBuffer, agenda->size.buffer );

        if ( !newMem )
            return;

        agenda->pBuffer     = newMem;
        agenda->size.buffer = newSize;
    }

    memcpy( AGENDA_GET_DATA( agenda ) + AGENDA_GET_USED_BYTES( agenda ), entry, agenda->size.element );
    AGENDA_GET_USED( agenda ) += 1;
}

void agenda_rmv( Agenda_t* agenda, f_compare_predicate compare, void* args ) {
    byte_t* found = agenda_search( agenda, compare, args );
    if ( !found )
        return;

    size_t dist = found - AGENDA_GET_DATA( agenda );
    for ( size_t i = dist + agenda->size.element; i < AGENDA_GET_USED_BYTES( agenda ); i++ )
        memcpy( AGENDA_GET_DATA( agenda ) + i - agenda->size.element, AGENDA_GET_DATA( agenda ) + i, agenda->size.element );

    AGENDA_GET_USED( agenda ) -= 1;
}

void* agenda_search( Agenda_t* agenda, f_compare_predicate compare, void* args ) {
    void* result = NULL;

    for ( size_t i = 0; i < AGENDA_GET_USED_BYTES( agenda ); i += agenda->size.element ) {
        result = AGENDA_GET_DATA( agenda ) + i;
        if ( compare( args, result ) )
            break;
        result = NULL;
    }

    return result;
}

void agenda_for_each( Agenda_t* agenda, f_for_each_callback callback, void* args ) {
    for ( size_t i = 0; i < AGENDA_GET_USED( agenda ); i++ )
        callback( args, AGENDA_GET_DATA( agenda ) + ( i * agenda->size.element ) );
}

void destroy_agenda( Agenda_t** agenda ) {
    if ( !agenda )
        return;

    Agenda_t* a = *agenda;

    if ( !a )
        return;

    if ( a->pBuffer )
        free( a->pBuffer );
    free( a );
}