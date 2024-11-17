#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "agenda.h"

#define ARRAY_INITIAL_SIZE 64
#define EXIT_FAILURE_IF_MSG( expr, ... ) \
    if ( expr ) {                        \
        fprintf( stderr, __VA_ARGS__ );  \
        return EXIT_FAILURE;             \
    }

enum MENU_CMD {
    MCMD_ADD_CONTACT,
    MCMD_RMV_CONTACT,
    MCMD_SEARCH_CONTACT,
    MCMD_LIST,
    MCMD_EXIT,

    MCMD_COUNT,
};

void menu_cmd_add( Agenda_t* agenda, char* buffer, size_t buffer_size );
void menu_cmd_rmv( Agenda_t* agenda, char* buffer, size_t buffer_size );
void menu_cmd_search( Agenda_t* agenda, char* buffer, size_t buffer_size );
void menu_cmd_list( Agenda_t* agenda );

void print_contact( void* args, AgendaEntry_t* name );

int main( void ) {
    Agenda_t* agenda = create_agenda( ARRAY_INITIAL_SIZE );
    EXIT_FAILURE_IF_MSG( !agenda, "[ERROR]: Não foi allocar memoria para o array" );

    bool running = true;
    enum MENU_CMD choice;
    char buffer[BUFSIZ];
    while ( running ) {
        printf(
            "1) Adicionar Contato\n"
            "2) Remover Contato\n"
            "3) Buscar Contato\n"
            "4) Listar\n"
            "5) Sair\n"
            "\n"
            ">>> " );
        fgets( buffer, BUFSIZ, stdin );
        if ( !sscanf( buffer, "%d", &choice ) ) {
            printf( "Não foi possivel intrepretar a entrada, por favor tente novamente\n" );
            continue;
        }
        choice -= 1;

        switch ( choice ) {
            case MCMD_ADD_CONTACT:
                menu_cmd_add( agenda, buffer, BUFSIZ );
                break;
            case MCMD_RMV_CONTACT:
                menu_cmd_rmv( agenda, buffer, BUFSIZ );
                break;
            case MCMD_SEARCH_CONTACT:
                menu_cmd_search( agenda, buffer, BUFSIZ );
                break;
            case MCMD_LIST:
                menu_cmd_list( agenda );
                break;
            case MCMD_EXIT:
                running = false;
                break;

            default:
                printf( "O command \"%d\" não existe, entre um commando valido por favor" );
                break;
        }
    }

    destroy_agenda( &agenda );
    return EXIT_SUCCESS;
}

void menu_cmd_add( Agenda_t* agenda, char* buffer, size_t buffer_size ) {
    AgendaEntry_t entry;
    do {
        fputs( "Entre o nome do contato: ", stdout );
        fgets( buffer, buffer_size, stdin );
        memset( entry.name, 0, sizeof( entry.name ) );

        for ( size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++ ) {
            if ( !isalpha( buffer[i] ) )
                continue;

            if ( j >= sizeof( entry.name ) )
                break;

            entry.name[j++] = tolower( buffer[i] );
        }

        entry.name[0] = toupper( entry.name[0] );
    } while ( !isgraph( buffer[0] ) );

    while ( true ) {
        fputs( "Entre a idade do contato: ", stdout );
        fgets( buffer, buffer_size, stdin );
        *strchr( buffer, '\n' ) = '\0';

        int temp = 0;
        if ( !sscanf( buffer, "%d", &temp ) ) {
            printf( "[ERRO]: Não foi possivel intrepretar a entrada \"%s\" como um numero.\n", buffer );
            continue;
        }

        if ( temp < 0 || temp > 120 ) {
            printf( "[ERRO]: A idade \"%d\" não é valida, a idade deve estar entre (0 - 120).\n", temp );
            continue;
        }

        entry.age = temp;
        break;
    }

    while ( true ) {
        fputs( "Entre o numero de telefone do contato (somente os numeros): ", stdout );
        fgets( buffer, buffer_size, stdin );
        size_t len = strnlen( buffer, buffer_size );

        if ( buffer[len - 1] == '\n' ) {
            len -= 1;
            buffer[len] = '\0';
        }

        if ( len != sizeof( ( (AgendaEntry_t*) NULL )->cellphone ) ) {
            printf( "[ERRO]: O numero informado não obedece o padrão, 9xxxxxxxx, tente novamente.\n" );
            continue;
        }

        // a goto would be so good right now :P
        bool error = false;
        for ( size_t i = 0; i < sizeof( ( (AgendaEntry_t*) NULL )->cellphone ); i++ ) {
            if ( !isdigit( buffer[i] ) ) {
                error = true;
                break;
            }
            entry.cellphone[i] = buffer[i];
        }
        if ( error ) {
            printf( "[ERRO]: Foi fornecido valores não numericos, tente novamente.\n" );
            continue;
        }

        break;
    }

    agenda_add( agenda, entry );
}

void menu_cmd_rmv( Agenda_t* agenda, char* buffer, size_t buffer_size ) {
    do {
        fputs( "Entre o nome do contato: ", stdout );
        fgets( buffer, buffer_size, stdin );
        for ( size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++ ) {
            if ( buffer[i] == '\n' ) {
                buffer[i] = '\0';
                break;
            } else if ( !isalpha( buffer[i] ) )
                continue;

            if ( j >= sizeof( ( (AgendaEntry_t*) NULL )->name ) )
                break;

            buffer[j++] = tolower( buffer[i] );
        }

        buffer[0] = toupper( buffer[0] );
    } while ( !isgraph( buffer[0] ) );

    agenda_rmv( agenda, buffer );
}

void menu_cmd_search( Agenda_t* agenda, char* buffer, size_t buffer_size ) {
    do {
        fputs( "Entre o nome do contato: ", stdout );
        fgets( buffer, buffer_size, stdin );
        for ( size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++ ) {
            if ( buffer[i] == '\n' ) {
                buffer[i] = '\0';
                break;
            } else if ( !isalpha( buffer[i] ) )
                continue;

            if ( j >= sizeof( ( (AgendaEntry_t*) NULL )->name ) )
                break;

            buffer[j++] = tolower( buffer[i] );
        }

        buffer[0] = toupper( buffer[0] );
    } while ( !isgraph( buffer[0] ) );

    AgendaEntry_t* entry = agenda_search( agenda, buffer );

    if ( !entry ) {
        printf( "O contato \"%s\", não foi encontrado.\n", buffer );
        return;
    }

    print_contact( NULL, entry );
}

void menu_cmd_list( Agenda_t* agenda ) {
    size_t i = 0;
    agenda_for_each( agenda, print_contact, &i );
}

void print_contact( void* args, AgendaEntry_t* entry ) {
    char age_str[4];
    itoa( entry->age, age_str, 10 );
    if ( args ) {
        size_t i = *( (size_t*) ( args ) );
        printf( "\t[%lu]: |%-10s|%3s|%c %.4s-%.4s|\n", i, (const char*) entry->name, age_str, entry->cellphone[0], entry->cellphone + 1, entry->cellphone + 5 );
    } else {
        printf( "\t|%-10s|%3s|%c %.4s-%.4s|\n", (const char*) entry->name, age_str, entry->cellphone[0], entry->cellphone + 1, entry->cellphone + 5 );
    }
}
