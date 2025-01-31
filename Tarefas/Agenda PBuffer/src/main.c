#include "agenda.h"
#include "arena.h"
#include "validators.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 255

void GetString(char *buffer) {
  fgets(buffer, BUFFERSIZE, stdin);

  char *temp = strchr(buffer, '\n');
  if (temp) {
    *temp = '\0';
  }
}

char *GetName(char *msg, char *buffer) {
  char *error = NULL;
  do {
    fputs(msg, stdout);
    GetString(buffer);
    error = IsInvalidName(buffer);
    if (error)
      puts(error);
  } while (error);

  FormatName(buffer);
  return buffer;
}

char *GetEmail(char *msg, char *buffer) {
  char *error = NULL;
  do {
    fputs(msg, stdout);
    GetString(buffer);
    error = IsInvalidEMail(buffer);
    if (error)
      puts(error);
  } while (error);
  return buffer;
}

void AddCommand(void *agenda, char *buffer);
void RmvCommand(void *agenda, char *buffer);
void SearchCommand(void *agenda, char *buffer);
void ListCommand(void *agenda);

void PrintEntry(void *entry);

int main(void) {
  void *agenda = CreateAgenda();

  int *choice = ARENA_ALLOC(sizeof(*choice));
  bool *running = ARENA_ALLOC(sizeof(*running));
  char *buffer = ARENA_ALLOC(BUFFERSIZE);

  if (!choice || !running) {
    fprintf(stderr, "[ERROR]: Failed to allocate locals on stack\n");
    return EXIT_FAILURE;
  }

  *ARENA_GETI(running) = true;

  while (*ARENA_GETI(running)) {
    printf("1- Adicionar Pessoa (Nome, Idade, email)\n"
           "2- Remover Pessoa\n"
           "3- Buscar Pessoa\n"
           "4- Listar todos\n"
           "5- Sair	\n\n");
    do {
      fputs(">>> ", stdout);
      GetString(ARENA_GETI(buffer));
    } while (sscanf(ARENA_GETI(buffer), "%d", ARENA_GETI(choice)) == 0);

    switch (*ARENA_GETI(choice)) {
    case 1:
      AddCommand(agenda, buffer);
      break;
    case 2:
      RmvCommand(agenda, buffer);
      break;
    case 3:
      SearchCommand(agenda, buffer);
      break;
    case 4:
      ListCommand(agenda);
      break;
    case 5:
      *ARENA_GETI(running) = false;
      break;
    default:
      break;
    }

    fputc('\n', stdout);
  }

  DestroyAgenda(&agenda);
  return EXIT_SUCCESS;
}

void AddCommand(void *agenda, char *name) {
  ARENA_SIHB(char, email, BUFFERSIZE);
  ARENA_SIH(uint8_t, age);
  ARENA_SIH(int, ageTemp);

  GetName("name: ", ARENA_GETI(name));

  do {
    fputs("age: ", stdout);
    GetString(ARENA_GETI(email));
  } while (sscanf(ARENA_GETI(email), "%d", ARENA_GETI(ageTemp)) == 0);

  *ARENA_GETI(age) = (uint8_t)(*ARENA_GETI(ageTemp));
  GetEmail("email: ", ARENA_GETI(email));

  AgendaAdd(agenda, name, age, email);
}

void RmvCommand(void *agenda, char *buffer) {
  GetName("name: ", ARENA_GETI(buffer));

  void *ret = AGENDA_REMOVE(agenda, buffer);

  if (ret) {
    printf("\"%s\" foi removido com successo!\n", ARENA_GETI(buffer));
  } else {
    printf("\"%s\" não foi encontrado, logo não foi removido!\n",
           ARENA_GETI(buffer));
  }
}

void SearchCommand(void *agenda, char *buffer) {
  GetName("name: ", ARENA_GETI(buffer));

  void *entry = AGENDA_SEARCH(agenda, buffer);
  if (entry) {
    fputc('\t', stdout);
    PrintEntry(entry);
    fputc('\n', stdout);
  } else {
    printf("Couldn't find the name \"%s\"\n", ARENA_GETI(buffer));
  }
}

void ListCommand(void *agenda) {
  ARENA_SIH(uint32_t, i);
  ARENA_SIH(void *, entry);

  for (*ARENA_GETI(i) = 0; true; *ARENA_GETI(i) += 1) {
    *ARENA_GETI(entry) = AGENDA_GET_ENTRY(agenda, *ARENA_GETI(i));
    if (!*ARENA_GETI(entry)) {
      break;
    }

    printf("\t[%u]: ", *ARENA_GETI(i));
    PrintEntry(*ARENA_GETI(entry));
    fputc('\n', stdout);
  }
}

void PrintEntry(void *entry) {
  printf("nome = ");
  uint8_t *entryData = entry;
  while (*entryData) {
    fputc(*entryData++, stdout);
  }
  printf(", age = %d, email = ", *++entryData);
  while (*entryData) {
    fputc(*++entryData, stdout);
  }
}
