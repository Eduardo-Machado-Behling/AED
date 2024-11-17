#include "unity.h"

#define ARRAY_EXPOSE_CLASS
#define NAME_MAX_LEN 64
#include "agenda.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

Agenda_t* ag;

void setUp( void ) {
    ag = create_agenda( 20 );
}

void tearDown( void ) {
    destroy_agenda( &ag );
}

struct CompareArgs_t {
    bool result;
    const char** expected;
    size_t i;
};

void compare( void* args, const char* name ) {
    struct CompareArgs_t* arg = args;

    if ( !arg->result )
        return;

    if ( strncmp( arg->expected[arg->i++], name, NAME_MAX_LEN ) != 0 )
        arg->result = false;
}

void test_proper_usage( void ) {
}

void test_tricky_usage( void ) {
}

int main( void ) {
    UNITY_BEGIN();

    return UNITY_END();
}
