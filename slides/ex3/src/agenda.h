#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void ( *f_for_each_callback )( void* args, void* elem );
typedef bool ( *f_compare_predicate )( void* args, void* elem );

typedef struct agenda_t Agenda_t;

Agenda_t* create_agenda( size_t initial_size, size_t elem_size );

void agenda_add( Agenda_t* agenda, void* entry );
void agenda_rmv( Agenda_t* agenda, f_compare_predicate compare, void* args );
void* agenda_search( Agenda_t* agenda, f_compare_predicate compare, void* args );
void agenda_for_each( Agenda_t* agenda, f_for_each_callback callback, void* args );

void destroy_agenda( Agenda_t** agenda );