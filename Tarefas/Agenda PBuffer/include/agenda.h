#ifndef AGENDA_H
#define AGENDA_H

#include <stdint.h>

#define AGENDA_SIZE ((sizeof(void *) * 2) + sizeof(const char *))
/*
 * Agenda Layout:
 * Offset (B) |     Type    | Content             | Reason            |
 *      0     | void *      | Darray Data         |  Data Storage     |
 *      8     | void *      | Darray prefix       |  Locals Vars      |
 *     16     | const char* | Name to earch      |  Function Args    |
 */

#define AGENDA_GET_ENTRY(AGENDA, INDEX)                                        \
  (*AgendaGetIndexArg(AGENDA) = INDEX, AgendaGetEntry(AGENDA))

#define AGENDA_SEARCH(AGENDA, NAME)                                            \
  (*AgendaGetSearchArg(AGENDA) = NAME, AgendaSearch(AGENDA))

#define AGENDA_REMOVE(AGENDA, NAME)                                            \
  (*AgendaGetSearchArg(AGENDA) = NAME, AgendaRemove(AGENDA))

void *CreateAgenda(void);

uint32_t *AgendaGetIndexArg(void *agenda);
const char **AgendaGetSearchArg(void *agenda);

// Need to use "AgendaGetIndexArg" to specify index,
// returns NULL on invalid index.
void *AgendaGetEntry(void *agenda);

// Need to use "AgendaGetSearchArg" to specify the name to search,
// returns NULL on not found.
void *AgendaSearch(void *agenda);

void AgendaAdd(void *agenda, const char *name, uint8_t *age, const char *email);

void *AgendaRemove(void *agenda);

void DestroyAgenda(void **agenda);

#endif
