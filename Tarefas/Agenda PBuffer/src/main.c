#include "agenda.h"
#include "arena.h"
#include "validators.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 255

void GetString( char* buffer );
char* GetName( char* msg, char* buffer );
char* GetEmail( char* msg, char* buffer );

void AddCommand( void* agenda, char* buffer );
void RmvCommand( void* agenda, char* buffer );
void SearchCommand( void* agenda, char* buffer );
void ListCommand( void* agenda );

void PrintEntry( void* entry );

int main( void ) {
    void* agenda = CreateAgenda();

    int* choice   = ARENA_ALLOC( sizeof( *choice ) );
    bool* running = ARENA_ALLOC( sizeof( *running ) );
    char* buffer  = ARENA_ALLOC( BUFFER_SIZE );

    if ( ARENA_INVALID( choice ) || ARENA_INVALID( running ) || ARENA_INVALID( buffer ) ) {
        fprintf( stderr, "[ERROR]: Failed to allocate locals on stack\n" );
        return EXIT_FAILURE;
    }

    *ARENA_GETI( running ) = true;

    while ( *ARENA_GETI( running ) ) {
        printf( "1- Adicionar Pessoa (Nome, Idade, email)\n"
                "2- Remover Pessoa\n"
                "3- Buscar Pessoa\n"
                "4- Listar todos\n"
                "5- Sair	\n\n" );
        do {
            fputs( ">>> ", stdout );
            GetString( ARENA_GETI( buffer ) );
        } while ( sscanf( ARENA_GETI( buffer ), "%d", ARENA_GETI( choice ) ) == 0 );

        switch ( *ARENA_GETI( choice ) ) {
            case 1:
                AddCommand( agenda, buffer );
                break;
            case 2:
                RmvCommand( agenda, buffer );
                break;
            case 3:
                SearchCommand( agenda, buffer );
                break;
            case 4:
                ListCommand( agenda );
                break;
            case 5:
                *ARENA_GETI( running ) = false;
                break;
            default:
                break;
        }

        fputc( '\n', stdout );
    }

    DestroyAgenda( &agenda );
    return EXIT_SUCCESS;
}

/*
=================
GetString

Gets a string from the stdin, needed because scanf is finicky.
=================
*/
void GetString( char* buffer ) {
    if ( !fgets( buffer, BUFFER_SIZE, stdin ) ) {
        return;
    }

    char* temp = strchr( buffer, '\n' );
    if ( temp ) {
        *temp = '\0';
    }
}

/*
=================
GetName

Prints a msg and gets a valid name according to
IsInvalidName() on validators.c
=================
*/
char* GetName( char* msg, char* buffer ) {
    char* error = NULL;
    do {
        fputs( msg, stdout );
        GetString( buffer );
        error = IsInvalidName( buffer );
        if ( error )
            puts( error );
    } while ( error );

    FormatName( buffer );
    return buffer;
}

/*
=================
GetEmail


Prints a msg and gets a valid email according to
IsInvalidEMail() on validators.c
=================
*/
char* GetEmail( char* msg, char* buffer ) {
    char* error = NULL;
    do {
        fputs( msg, stdout );
        GetString( buffer );
        error = IsInvalidEMail( buffer );
        if ( error )
            puts( error );
    } while ( error );
    return buffer;
}

/*
=================
AddCommand


Gets from stdin the necessary data to add a entry
to the Agenda.
=================
*/
void AddCommand( void* agenda, char* name ) {
    ARENA_SIHB( char, email, BUFFER_SIZE );
    ARENA_SIH( uint8_t, age );
    ARENA_SIH( int, ageTemp );

    GetName( "name: ", ARENA_GETI( name ) );

    do {
        fputs( "age: ", stdout );
        GetString( ARENA_GETI( email ) );
    } while ( sscanf( ARENA_GETI( email ), "%d", ARENA_GETI( ageTemp ) ) == 0 );

    *ARENA_GETI( age ) = (uint8_t) ( *ARENA_GETI( ageTemp ) );
    GetEmail( "email: ", ARENA_GETI( email ) );

    AgendaAdd( agenda, name, age, email );
}

void RmvCommand( void* agenda, char* buffer ) {
    GetName( "name: ", ARENA_GETI( buffer ) );

    void* ret = AGENDA_REMOVE( agenda, buffer );

    if ( ret ) {
        printf( "\"%s\" foi removido com successo!\n", ARENA_GETI( buffer ) );
    } else {
        printf( "\"%s\" não foi encontrado, logo não foi removido!\n",
                ARENA_GETI( buffer ) );
    }
}

/*
=================
SearchCommand

Gets a name from stdin and searchs for it in the Agenda.
=================
*/
void SearchCommand( void* agenda, char* buffer ) {
    GetName( "name: ", ARENA_GETI( buffer ) );

    void* entry = AGENDA_SEARCH( agenda, buffer );
    if ( entry ) {
        fputc( '\t', stdout );
        PrintEntry( entry );
        fputc( '\n', stdout );
    } else {
        printf( "\"%s\" não foi encontrado!\n",
                ARENA_GETI( buffer ) );
    }
}

/*
=================
ListCommand


Lists all entries in the Agenda.
=================
*/
void ListCommand( void* agenda ) {
    ARENA_SIH( uint32_t, i );
    ARENA_SIH( void*, entry );

    for ( *ARENA_GETI( i ) = 0; true; *ARENA_GETI( i ) += 1 ) {
        *ARENA_GETI( entry ) = AGENDA_GET_ENTRY( agenda, *ARENA_GETI( i ) );
        if ( !*ARENA_GETI( entry ) ) {
            break;
        }

        printf( "\t[%u]: ", *ARENA_GETI( i ) );
        PrintEntry( *ARENA_GETI( entry ) );
        fputc( '\n', stdout );
    }
}

/*
=================
PrintEntry


Print the data in a entry.
=================
*/
void PrintEntry( void* entry ) {
    printf( "nome = " );
    uint8_t* entryData = entry;
    while ( *entryData ) {
        fputc( *entryData++, stdout );
    }
    printf( ", age = %d, email = ", *++entryData );
    while ( *entryData ) {
        fputc( *++entryData, stdout );
    }
}
