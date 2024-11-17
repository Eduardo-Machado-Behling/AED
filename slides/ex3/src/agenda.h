#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct agenda_entry_t AgendaEntry_t;

typedef void ( *f_for_each_callback )( void* args, AgendaEntry_t* entry );

#ifdef AGENDA_EXPOSE_CLASS
typedef struct agenda_t {
    uint8_t* buffer;
    size_t size;
    size_t used;
} Agenda_t;
#else
typedef struct agenda_t Agenda_t;
#endif

typedef struct agenda_entry_t {
    char name[10];
    char cellphone[9];
    uint8_t age;
} AgendaEntry_t;

Agenda_t* create_agenda( size_t initial_size );

void agenda_add( Agenda_t* agenda, AgendaEntry_t entry );
void agenda_rmv( Agenda_t* agenda, const char* name );
AgendaEntry_t* agenda_search( Agenda_t* agenda, const char* name );
void agenda_for_each( Agenda_t* agenda, f_for_each_callback callback, void* args );

void destroy_agenda( Agenda_t** agenda );