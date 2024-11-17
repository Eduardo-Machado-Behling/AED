#define ARRAY_EXPOSE_CLASS
#include "array.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MIN( a, b ) ( a < b ? a : b )

StringArray_t* create_string_array( size_t initial_size ) {
    StringArray_t* array = malloc( sizeof( StringArray_t ) );
    if ( !array )
        return NULL;

    array->size  = initial_size;
    array->used  = 0;
    array->array = malloc( initial_size );
    if ( !array->array ) {
        destroy_string_array( &array );
        return NULL;
    }

    return array;
}

void string_array_add( StringArray_t* array, const char* name, size_t max_len ) {
    size_t len = strnlen( name, max_len ) + 1;

    size_t available_size = array->size - array->used;

    /*
     * We assume that the name doesn't have non alphanumeric characters and
     * allocate memory with this basis, but afterwards we strip the those chars,
     * all in order to have name be const :P, I could have a buffer to put the stripped name
     * but seems wasteful.
     */
    if ( available_size < len ) {
        array->size += MIN( array->size * 2, 64 );

        void* new_mem = realloc( array->array, array->size );
        if ( !new_mem )
            return;

        array->array = new_mem;
    }

    // very fun indeed kkkk
    int ( *fun )( int ) = toupper;
    size_t j            = array->used;

    for ( size_t i = 0; i < len; i++ ) {
        if ( !isalpha( name[i] ) )
            continue;

        array->array[j++] = fun( name[i] );

        fun = tolower;
    }
    array->array[j] = '\0';

    array->used += j - array->used;
}

void string_array_rmv( StringArray_t* array, const char* name ) {
    size_t cmp = 0;
    size_t i   = 0;
    for ( ; i < array->used && isalpha( name[cmp] ); i++ ) {
        char curr_char_name = cmp == 0 ? toupper( name[cmp] ) : tolower( name[cmp] );
        if ( array->array[i] == curr_char_name )
            cmp++;
        else
            cmp = 0;
    }

    // this means that it uses a invalid character (non alpha) (ex: /]~;´[])
    if ( isgraph( name[cmp] ) )
        return;

    if ( i != array->used ) {
        // Avoids erasing names with prefixes,
        // this ensures that the name compared in the array ended
        if ( islower( array->array[i] ) )
            return;
    } else if ( cmp != 0 ) { // Case when the name to erase is the last one
        array->array[i - cmp] = '\0';
        return;
    }

    for ( ; i < array->used; i++ ) {
        array->array[i - cmp] = array->array[i];
    }
    array->array[i - cmp] = '\0';
    array->used -= cmp;
}

void string_array_for_each( StringArray_t* array, f_name_callback callback, void* args ) {
    char temp_holder = '\0';
    char* str        = NULL;

    for ( size_t i = 0; i < array->used; i++ ) {
        if ( !isupper( array->array[i] ) )
            continue;

        if ( str ) {
            temp_holder     = array->array[i];
            array->array[i] = '\0';
            callback( args, str );
            array->array[i] = temp_holder;

            str = array->array + i;
        } else {
            str = array->array + i;
        }
    }
    // for the last string
    callback( args, str );
}

void destroy_string_array( StringArray_t** string_array ) {
    if ( !string_array )
        return;

    StringArray_t* array = *string_array;
    if ( !array )
        return;

    if ( array->array )
        free( array->array );
    free( array );
}