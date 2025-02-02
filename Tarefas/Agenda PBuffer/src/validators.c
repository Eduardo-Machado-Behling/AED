#include "validators.h"
#include "common.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
=================
FormatName

Formats inplace the name buffer, to have each word capitalized.
=================
*/
void FormatName( char* name ) {
    *name = toupper( *name );
    while ( 1 ) {
        name = strchr( name, ' ' );
        if ( !name ) {
            break;
        }
        if ( *++name != '\0' ) {
            *name = toupper( *name );
        }
    }
}

/*
=================
IsInvalidEMail

Is equivalent to the RegEx /[A-Za-z]*@[A-Za-z]*(\.[A-Za-z){1,3})+/
=================
*/
char* IsInvalidEMail( char* email ) {
    // Equivale a RegEx /[A-Za-z]*@/
    char* it = email;
    for ( ; *it && *it != '@' && isalpha( *it ); it++ ) {
        ;
    }
    if ( *it++ != '@' ) {
        return "Não foi encontrado o \"@\"!";
    }

    // Equivale a RegEx /[A-Za-z]*\./
    char* j = it;
    for ( ; *it && isalpha( *it ) && *it != '.'; it++ ) {
    }

    if ( GET_OFFSET( it, j ) == 0 ) {
        return "Após o \"@\" deve ter pelo menos 1 caracter!";
    } else if ( *it != '.' ) {
        return "Não foi encontrado o \".\"!";
    }

    // Equivale a RegEx /(\.[A-Za-z]+)+$/
    while ( *it ) {
        for ( j = ++it; *it && isalpha( *it ) && *it != '.'; it++ ) {
            ;
        }

        if ( GET_OFFSET( it, j ) == 0 ) {
            return "Após o \".\" deve ter pelo menos 1 caracter!";
        }
    }
    return NULL;
}

/*
=================
IsInvalidName

Is Equivalent to the RegEx /[A-Za-z ]+/
=================
*/
char* IsInvalidName( char* name ) {
    if ( *name == '\0' ) {
        return "Nome tem que ter tamanho maior que 0!";
    }

    char* it = name;
    for ( ; *it; it++ ) {
        if ( !isalpha( *it ) && *it != ' ' ) {
            return "O nome só pode conter caracteres do alfabeto [A-Za-z ]!";
        }
    }

    // Trim right side spaces ' '
    for ( --it; *it == ' '; --it ) {
        *it = '\0';
    }

    return NULL;
}
