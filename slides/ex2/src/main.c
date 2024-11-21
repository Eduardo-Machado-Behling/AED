#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define ARRAY_INITIAL_SIZE 10
#define NAME_MAX_LEN 64
#define EXIT_FAILURE_IF_MSG(expr, ...)                                         \
  if (expr) {                                                                  \
    fprintf(stderr, __VA_ARGS__);                                              \
    return EXIT_FAILURE;                                                       \
  }

enum MENU_CMD {
  MCMD_ADD_NAME = 1,
  MCMD_RMV_NAME,
  MCMD_LIST,
  MCMD_EXIT,
};

void print_name(void *args, const char *name);

int main(void) {
  StringArray_t *array = create_string_array(ARRAY_INITIAL_SIZE);
  EXIT_FAILURE_IF_MSG(!array, "[ERROR]: Não foi allocar memoria para o array");

  bool running = true;
  enum MENU_CMD choice;
  char buffer[BUFSIZ];
  while (running) {
    printf("1) Adicionar Nome\n"
           "2) Remover Nome\n"
           "3) Listar\n"
           "4) Sair\n"
           "\n"
           ">>> ");
    fgets(buffer, BUFSIZ, stdin);
    if (!sscanf(buffer, "%d", &choice)) {
      printf("Não foi possivel intrepretar a entrada, por favor tente "
             "novamente\n");
      continue;
    }

    switch (choice) {
    case MCMD_ADD_NAME:
      printf("Entre o nome a ser adicionado: ");
      fgets(buffer, NAME_MAX_LEN, stdin);
      string_array_add(array, buffer, NAME_MAX_LEN);
      break;
    case MCMD_RMV_NAME:
      printf("Entre o nome a ser removido: ");
      fgets(buffer, NAME_MAX_LEN, stdin);
      string_array_rmv(array, buffer);
      break;
    case MCMD_LIST:
      string_array_for_each(array, print_name, NULL);
      break;
    case MCMD_EXIT:
      running = false;
      break;

    default:
      printf("O command \"%d\" não existe, entre um commando valido por favor");
      break;
    }
  }

  destroy_string_array(&array);
  return EXIT_SUCCESS;
}

void print_name(void *args, const char *name) { printf("\t\t%s\n", name); }
