#define AGENDA_EXPOSE_CLASS
#include "agenda.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN( a, b ) ( a < b ? a : b )

Agenda_t* create_agenda( size_t initial_size ) {
    Agenda_t* agenda = malloc( sizeof( Agenda_t ) );

    agenda->used = 0;
    agenda->size = initial_size;

    agenda->buffer = malloc( agenda->size );
    if ( !agenda->buffer )
        destroy_agenda( &agenda );

    return agenda;
}

void agenda_add( Agenda_t* agenda, AgendaEntry_t entry ) {
    size_t available_size = agenda->size - agenda->used;

    if ( available_size < sizeof( AgendaEntry_t ) ) {
        size_t new_size = agenda->size + MIN( agenda->size * 2, 255 );
        void* new_mem   = realloc( agenda->buffer, agenda->size );

        if ( !new_mem )
            return;

        agenda->buffer = new_mem;
        agenda->size   = new_size;
    }

    memcpy( agenda->buffer + agenda->used, (void*) &entry, sizeof( AgendaEntry_t ) );
    agenda->used += sizeof( AgendaEntry_t );
}

void agenda_rmv( Agenda_t* agenda, const char* name ) {
    bool found = false;
    size_t i   = 0;
    for ( ; i < agenda->used && !found; i += sizeof( AgendaEntry_t ) ) {
        if ( strncmp( agenda->buffer + i, name, sizeof( ( (AgendaEntry_t*) NULL )->name ) ) == 0 )
            found = true;
    }

    if ( !found )
        return;

    for ( ; i < agenda->used; i++ )
        agenda->buffer[i] = agenda->buffer[i + sizeof( AgendaEntry_t )];

    agenda->used -= sizeof( AgendaEntry_t );
}

AgendaEntry_t* agenda_search( Agenda_t* agenda, const char* name ) {
    bool found = false;
    size_t i   = 0;
    for ( ; i < agenda->used && !found; i += sizeof( AgendaEntry_t ) ) {
        if ( strncmp( agenda->buffer + i, name, sizeof( ( (AgendaEntry_t*) NULL )->name ) ) == 0 )
            found = true;
    }

    if ( !found )
        return NULL;

    return (AgendaEntry_t*) ( agenda->buffer + i );
}

void agenda_for_each( Agenda_t* agenda, f_for_each_callback callback, void* args ) {
    size_t i = 0;
    for ( ; i < agenda->used; i += sizeof( AgendaEntry_t ) )
        callback( args, (AgendaEntry_t*) ( agenda->buffer + i ) );
}

void destroy_agenda( Agenda_t** agenda ) {
    if ( !agenda )
        return;

    Agenda_t* a = *agenda;

    if ( !a )
        return;

    if ( a->buffer )
        free( a->buffer );
    free( a );
}