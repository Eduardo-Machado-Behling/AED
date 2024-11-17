#pragma once

#include <stddef.h>

typedef void ( *f_name_callback )( void* args, const char* name );

#ifdef ARRAY_EXPOSE_CLASS
typedef struct string_array_t {
    char* array;
    size_t size;
    size_t used;
} StringArray_t;
#else
typedef struct string_array_t StringArray_t;
#endif

StringArray_t* create_string_array( size_t initial_size );

void string_array_add( StringArray_t* array, const char* name, size_t max_len );
void string_array_rmv( StringArray_t* array, const char* name );
void string_array_for_each( StringArray_t* array, f_name_callback callback, void* args );

void destroy_string_array( StringArray_t** string_array );