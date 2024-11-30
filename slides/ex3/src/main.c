#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"

#define ARRAY_INITIAL_SIZE 64
#define CELLPHONE_LEN 9
#define EXIT_FAILURE_IF_MSG(expr, ...)                                         \
  if (expr) {                                                                  \
    fprintf(stderr, __VA_ARGS__);                                              \
    return EXIT_FAILURE;                                                       \
  }

enum menuCmd_t {
  MCMD_ADD_CONTACT,
  MCMD_RMV_CONTACT,
  MCMD_SEARCH_CONTACT,
  MCMD_LIST,
  MCMD_EXIT,

  MCMD_COUNT,
};

struct AgendaEntry_t {
  char name[10];
  uint8_t age;
  uint32_t cellphone;
};

void MenuCmdAdd(Agenda_t *agenda, char *buffer, size_t buffer_size);
void MenuCmdRmv(Agenda_t *agenda, char *buffer, size_t buffer_size);
void MenuCmdSearch(Agenda_t *agenda, char *buffer, size_t buffer_size);
void MenuCmdList(Agenda_t *agenda);

void PrintContact(void *args, void *elem);
bool CompareByName(void *args, void *elem);

int main(void) {
  Agenda_t *agenda =
      create_agenda(ARRAY_INITIAL_SIZE, sizeof(struct AgendaEntry_t));
  EXIT_FAILURE_IF_MSG(!agenda, "[ERROR]: Não foi allocar memoria para o array");

  bool running = true;
  enum menuCmd_t choice;
  char buffer[BUFSIZ];
  while (running) {
    printf("1) Adicionar Contato\n"
           "2) Remover Contato\n"
           "3) Buscar Contato\n"
           "4) Listar\n"
           "5) Sair\n"
           "\n"
           ">>> ");
    fgets(buffer, BUFSIZ, stdin);
    if (!sscanf(buffer, "%d", &choice)) {
      printf("Não foi possivel intrepretar a entrada, por favor tente "
             "novamente\n");
      continue;
    }
    choice -= 1;

    switch (choice) {
    case MCMD_ADD_CONTACT:
      MenuCmdAdd(agenda, buffer, BUFSIZ);
      break;
    case MCMD_RMV_CONTACT:
      MenuCmdRmv(agenda, buffer, BUFSIZ);
      break;
    case MCMD_SEARCH_CONTACT:
      MenuCmdSearch(agenda, buffer, BUFSIZ);
      break;
    case MCMD_LIST:
      MenuCmdList(agenda);
      break;
    case MCMD_EXIT:
      running = false;
      break;

    default:
      printf("O command \"%d\" não existe, entre um commando valido por favor");
      break;
    }

    printf("\n\n");
  }

  destroy_agenda(&agenda);
  return EXIT_SUCCESS;
}

void MenuCmdAdd(Agenda_t *agenda, char *buffer, size_t buffer_size) {
  struct AgendaEntry_t entry;
  do {
    fputs("Entre o nome do contato: ", stdout);
    fgets(buffer, buffer_size, stdin);
    memset(entry.name, 0, sizeof(entry.name));

    for (size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++) {
      if (!isalpha(buffer[i]))
        continue;

      if (j >= sizeof(entry.name))
        break;

      entry.name[j++] = tolower(buffer[i]);
    }

    entry.name[0] = toupper(entry.name[0]);
  } while (!isgraph(buffer[0]));

  while (true) {
    fputs("Entre a idade do contato: ", stdout);
    fgets(buffer, buffer_size, stdin);
    *strchr(buffer, '\n') = '\0';

    int temp = 0;
    if (!sscanf(buffer, "%d", &temp)) {
      printf("[ERRO]: Não foi possivel intrepretar a entrada \"%s\" como um "
             "numero.\n",
             buffer);
      continue;
    }

    if (temp < 0 || temp > 120) {
      printf("[ERRO]: A idade \"%d\" não é valida, a idade deve estar entre (0 "
             "- 120).\n",
             temp);
      continue;
    }

    entry.age = temp;
    break;
  }

  while (true) {
    fputs("Entre o numero de telefone do contato (somente os numeros): ",
          stdout);
    fgets(buffer, buffer_size, stdin);
    size_t len = strnlen(buffer, buffer_size);

    if (buffer[len - 1] == '\n') {
      len -= 1;
      buffer[len] = '\0';
    }

    if (len != CELLPHONE_LEN) {
      printf("[ERRO]: O numero informado não obedece o padrão, 9xxxxxxxx, "
             "tente novamente.\n");
      continue;
    }

    // a goto would be so good right now :P
    bool error = false;
    for (size_t i = 0; i < CELLPHONE_LEN; i++) {
      if (!isdigit(buffer[i])) {
        error = true;
        break;
      }
    }
    if (error) {
      printf("[ERRO]: Foi fornecido valores não numericos, tente novamente.\n");
      continue;
    }

    entry.cellphone = atoll(buffer);
    break;
  }

  agenda_add(agenda, &entry);
}

void MenuCmdRmv(Agenda_t *agenda, char *buffer, size_t buffer_size) {
  do {
    fputs("Entre o nome do contato: ", stdout);
    fgets(buffer, buffer_size, stdin);
    for (size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++) {
      if (buffer[i] == '\n') {
        buffer[i] = '\0';
        break;
      } else if (!isalpha(buffer[i]))
        continue;

      if (j >= sizeof(((struct AgendaEntry_t *)NULL)->name))
        break;

      buffer[j++] = tolower(buffer[i]);
    }

    buffer[0] = toupper(buffer[0]);
  } while (!isgraph(buffer[0]));

  agenda_rmv(agenda, CompareByName, buffer);
}

void MenuCmdSearch(Agenda_t *agenda, char *buffer, size_t buffer_size) {
  do {
    fputs("Entre o nome do contato: ", stdout);
    fgets(buffer, buffer_size, stdin);
    for (size_t i = 0, j = 0; i < buffer_size && buffer[i]; i++) {
      if (buffer[i] == '\n') {
        buffer[i] = '\0';
        break;
      } else if (!isalpha(buffer[i]))
        continue;

      if (j >= sizeof(((struct AgendaEntry_t *)NULL)->name))
        break;

      buffer[j++] = tolower(buffer[i]);
    }

    buffer[0] = toupper(buffer[0]);
  } while (!isgraph(buffer[0]));

  struct AgendaEntry_t *entry = agenda_search(agenda, CompareByName, buffer);

  if (!entry) {
    printf("O contato \"%s\", não foi encontrado.\n", buffer);
    return;
  }

  PrintContact(NULL, entry);
}

void MenuCmdList(Agenda_t *agenda) {
  size_t i = 0;
  agenda_for_each(agenda, PrintContact, &i);
}

void PrintContact(void *args, void *elem) {
  struct AgendaEntry_t *entry = elem;
  char strbuff[32];
  sprintf(strbuff, "%u", entry->age);
  sprintf(strbuff + 4, "%u", entry->cellphone);
  if (args) {
    size_t *i = (size_t *)(args);
    printf("\t[%lu]: |%-10s|%3s|%c %.4s-%.4s|\n", (*i)++,
           (const char *)entry->name, strbuff, strbuff[4], strbuff + 5,
           strbuff + 9);
  } else {
    printf("\t|%-10s|%3s|%c %.4s-%.4s|\n", (const char *)entry->name, strbuff,
           strbuff[4], strbuff + 5, strbuff + 9);
  }
}

bool CompareByName(void *args, void *elem) {
  const char *name = args;
  struct AgendaEntry_t *entry = elem;

  return strncmp(name, entry->name, sizeof(entry->name)) == 0;
}
